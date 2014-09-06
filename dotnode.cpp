#include "dotnode.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"

DotNode::DotNode(ExprNode *base, std::string member_name) : base(base), member_name(member_name), base_type(nullptr), member(nullptr), code_obj(new CodeObject()) 
{

}

DotNode::~DotNode() 
{
   	delete base; 
	delete code_obj; 
}

void DotNode::check()
{
	base -> check();

	auto _base_type = base -> getType();

	base_type = dynamic_cast<const StructSymbol*>(_base_type);

	if ( base_type == nullptr )
		throw SemanticError("left side of '.' is not an instance of struct.");

	member = dynamic_cast<VariableSymbol*>(base_type -> resolveMember(member_name));

	if ( member == nullptr )
		throw SemanticError(member_name + " is not member of " + base_type -> getName());
}

CodeObject& DotNode::gen()
{    
	code_obj -> emit(base -> gen().getCode());
	
	auto member_type = member -> getType();

	if ( member_type -> getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
	{
		auto ov_func = static_cast<const OverloadedFunctionSymbol*>(member_type);

		auto ov_func_type_info = ov_func -> getTypeInfo();

		if ( ov_func_type_info.overloads.size() > 1 )
		{
			if ( type_hint == nullptr )
				throw SemanticError("multiple overloads of " + base_type -> getName() + "::" + member -> getName());

			member = new VariableSymbol(member_name, ov_func_type_info.symbols[static_cast<const FunctionSymbol*>(type_hint) -> function_type_info]);
		} else
			member = new VariableSymbol(ov_func -> getName(), std::begin(ov_func_type_info.symbols) -> second);
	}
	else
		code_obj -> emit("lea rax, [rax - " + std::to_string(const_cast<StructSymbol*>(base_type) -> getVarAlloc().getAddress(member)) + "]");

	return *code_obj;
}
	
std::vector<AST*> DotNode::getChildren() const 
{
   	return {base}; 
}

AST* DotNode::copyTree() const
{
   	return new DotNode(static_cast<ExprNode*>(base -> copyTree()), member_name); 
}

const Type* DotNode::getType() const 
{
   	return member -> getType(); 
}

bool DotNode::isLeftValue() const
{
   	return true; 
}

void DotNode::freeTempSpace()
{

}

bool DotNode::isCompileTimeExpr() const
{
	return false;
}

boost::optional<int> DotNode::getCompileTimeValue() const
{
	return boost::none;
}
