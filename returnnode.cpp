#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), copy_call_info(), enclosing_func(nullptr) { }

ReturnNode::~ReturnNode() { delete expr; }

void ReturnNode::gen(const TemplateInfo& template_info)
{
	if ( enclosing_func->getTypeInfo().return_type.is_ref )
	{
		if ( !expr->isLeftValue() )
			throw SemanticError("cannot initialize " + enclosing_func->getTypeInfo().return_type.getName() + " with value of type " + expr->getType().getName());

		expr->gen(template_info);
	}
	else
	{
		CodeGen::genCallCode(copy_call_info, {expr}, template_info,
				[&]() { CodeGen::emit("mov r9, [rbp]"); CodeGen::emit("lea rax, [r9 - " + std::to_string(GlobalConfig::int_size) + "]"); }
		);
	}

    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
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
