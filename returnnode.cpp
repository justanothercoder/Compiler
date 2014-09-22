#include "returnnode.hpp"
#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "exprnode.hpp"
#include "globalconfig.hpp"
#include "structsymbol.hpp"
#include "copytypevisitor.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), enclosing_func(nullptr) 
{

}

ReturnNode::~ReturnNode() 
{ 
	delete expr; 
}

CodeObject& ReturnNode::gen()
{
	if ( enclosing_func -> return_type -> isReference() )
	{
		if ( !expr -> isLeftValue() )
			throw SemanticError("cannot initialize " + enclosing_func -> return_type -> getName() + " with value of type " + expr -> getType() -> getName());

		code_obj.emit(expr -> gen().getCode());
	}
	else
	{
		CodeObject return_place;

		auto& params_types = enclosing_func -> function_type_info.params_types;
		int addr = std::accumulate(std::begin(params_types), std::end(params_types), 0, [](int acc, const Type *type)
		{
			return acc += type -> getSize();
		}) + 2 * GlobalConfig::int_size; // expr -> getType().getSize();

		return_place.emit("mov rax, [rbp + " + std::to_string(addr) + "]");

		if ( expr -> getType() -> getTypeKind() != TypeKind::POINTER )
			code_obj.genCallCode(copy_call_info, {expr}, return_place, false);
		else
		{
			code_obj.emit(expr -> gen().getCode());
//			code_obj.emit("mov rbx, [rax]");
			code_obj.emit("lea rbx, [rax]");
			code_obj.emit("mov rax, [rbp + " + std::to_string(addr) + "]");
			code_obj.emit(CopyTypeVisitor("rbx", "rax").getCopyCode(expr -> getType()).getCode());
		}
	}

	if ( !enclosing_func -> return_type -> isReference() )
		code_obj.emit("mov rax, [rax]");

    code_obj.emit("mov rsp, rbp");
    code_obj.emit("pop rbp");
    code_obj.emit("ret");

	return code_obj;
}

void ReturnNode::check()
{
	auto _scope = scope;
	while ( _scope != nullptr && dynamic_cast<FunctionSymbol*>(_scope) == nullptr )
		_scope = _scope -> getEnclosingScope();

	if ( _scope == nullptr )
		throw SemanticError("return is not a in a function");

	enclosing_func = dynamic_cast<FunctionSymbol*>(_scope);

    expr -> check();

//	copy_call_info = CallHelper::callCheck(expr -> getType() -> getTypeName(), static_cast<const StructSymbol*>(expr -> getType().type), {expr});
	if ( expr -> getType() -> getTypeKind() != TypeKind::POINTER )
//		copy_call_info = CallHelper::callCheck(expr -> getType() -> getName(), static_cast<const StructSymbol*>(expr -> getType()), {expr});
		copy_call_info = CallHelper::callCheck(expr -> getType() -> getUnqualifiedType() -> getName(), static_cast<const StructSymbol*>(expr -> getType() -> getSymbol()), {expr});
}
	
void ReturnNode::define() 
{
   	expr -> define(); 
}

AST* ReturnNode::copyTree() const 
{
   	return new ReturnNode(static_cast<ExprNode*>(expr -> copyTree())); 
}

std::vector<AST*> ReturnNode::getChildren() const 
{
   	return {expr}; 
}

std::string ReturnNode::toString() const
{
	return "return " + expr -> toString() + ";";
}
