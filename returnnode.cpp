#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), copy_call_info(), enclosing_func(nullptr), code_obj() { }

ReturnNode::~ReturnNode() { delete expr; }

CodeObject& ReturnNode::gen()
{
	if ( enclosing_func -> function_type_info.return_type.is_ref )
	{
		if ( !expr -> isLeftValue() )
			throw SemanticError("cannot initialize " + enclosing_func -> function_type_info.return_type.getName() + " with value of type " + expr -> getType().getName());

		code_obj.emit(expr -> gen().getCode());
	}
	else
	{
		CodeObject return_place;

		return_place.emit("mov r9, [rbp]");
	   	return_place.emit("lea rax, [r9 - " + std::to_string(GlobalConfig::int_size) + "]");

		code_obj.genCallCode(copy_call_info, {expr}, return_place, false);
	}

	if ( !enclosing_func -> function_type_info.return_type.is_ref )
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

	enclosing_func = dynamic_cast<FunctionSymbol*>(scope);

    expr -> check();

	copy_call_info = CallHelper::callCheck(expr -> getType().getTypeName(), static_cast<StructSymbol*>(expr -> getType().type), {expr});
}
	
void ReturnNode::define() { expr -> define(); }

AST* ReturnNode::copyTree() const { return new ReturnNode(static_cast<ExprNode*>(expr -> copyTree())); }

vector<AST*> ReturnNode::getChildren() const { return {expr}; }
