#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr) { }

ReturnNode::~ReturnNode() { delete expr; }

void ReturnNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> _expr)
{
    expr->gen(template_sym, _expr);

    CodeGen::pushOnStack(expr->getType()->getSize(), this->getScope()->getFreeAddress());
    CodeGen::emit("lea rax, [rsp - " + std::to_string(this->getScope()->getFreeAddress()) + "]");

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

void ReturnNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> _expr) { expr->define(template_sym, _expr); }

AST* ReturnNode::copyTree() const { return new ReturnNode(static_cast<ExprNode*>(expr->copyTree())); }

vector<AST*> ReturnNode::getChildren() const { return {expr}; }
