#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr), copy_call_info() { }

ReturnNode::~ReturnNode() { delete expr; }

void ReturnNode::gen(const TemplateInfo& template_info)
{
	CodeGen::genCallCode(copy_call_info, {expr}, template_info,
			[&]() { CodeGen::emit("lea rax, [" + copy_call_info.callee->getScopedTypedName() + "]"); },
			[&]() { CodeGen::emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddressForLocal()) + "]"); }
	);

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

    expr->check(template_info);

	copy_call_info = CallHelper::callCheck(expr->getType().getTypeName(), static_cast<StructSymbol*>(expr->getType().type), {expr}, template_info);
}
	
void ReturnNode::define(const TemplateInfo& template_info) { expr->define(template_info); }

AST* ReturnNode::copyTree() const { return new ReturnNode(static_cast<ExprNode*>(expr->copyTree())); }

vector<AST*> ReturnNode::getChildren() const { return {expr}; }
