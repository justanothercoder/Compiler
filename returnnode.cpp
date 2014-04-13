#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), copy_call_info(), enclosing_func(nullptr), code_obj() { }

ReturnNode::~ReturnNode() { delete expr; }

CodeObject& ReturnNode::gen(const TemplateInfo& template_info)
{
	if ( enclosing_func->getTypeInfo().return_type.is_ref )
	{
		if ( !expr->isLeftValue() )
			throw SemanticError("cannot initialize " + enclosing_func->getTypeInfo().return_type.getName() + " with value of type " + expr->getType().getName());

		code_obj.emit(expr->gen(template_info).getCode());
	}
	else
	{
		CodeObject return_place;

		return_place.emit("mov r9, [rbp]");
	   	return_place.emit("lea rax, [r9 - " + std::to_string(GlobalConfig::int_size) + "]");

		code_obj.genCallCode(copy_call_info, {expr}, template_info, return_place);
	}

    code_obj.emit("mov rsp, rbp");
    code_obj.emit("pop rbp");
    code_obj.emit("ret");

	return code_obj;
}

void ReturnNode::check(const TemplateInfo& template_info)
{
	auto scope = getScope();
	while ( scope != nullptr && dynamic_cast<FunctionSymbol*>(scope) == nullptr )
		scope = scope->getEnclosingScope();

	if ( scope == nullptr )
		throw SemanticError("return is not a in a function");

	enclosing_func = dynamic_cast<FunctionSymbol*>(scope);

    expr->check(template_info);

	copy_call_info = CallHelper::callCheck(expr->getType().getTypeName(), static_cast<StructSymbol*>(expr->getType().type), {expr}, template_info);
}
	
void ReturnNode::define(const TemplateInfo& template_info) { expr->define(template_info); }

AST* ReturnNode::copyTree() const { return new ReturnNode(static_cast<ExprNode*>(expr->copyTree())); }

vector<AST*> ReturnNode::getChildren() const { return {expr}; }
