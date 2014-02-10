#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr) { }

void ReturnNode::build_scope()
{
    expr->setScope(this->getScope());
    expr->build_scope();
}

void ReturnNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    this->expr->gen(template_sym, expr);

    CodeGen::pushOnStack(this->expr->getType()->getSize(), GlobalConfig::int_size);
    CodeGen::emit("lea rax, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");

    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
}

void ReturnNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> _expr)
{
	Scope *sc = this->getScope();
	while ( sc != nullptr && dynamic_cast<FunctionSymbol*>(sc) == nullptr )
		sc = sc->getEnclosingScope();

	if ( sc == nullptr )
		throw SemanticError("return is not a in a function");

    expr->check(template_sym, _expr);
}

void ReturnNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    this->expr->define(template_sym, expr);
}

AST* ReturnNode::copyTree() const
{
    return new ReturnNode(static_cast<ExprNode*>(expr->copyTree()));
}
