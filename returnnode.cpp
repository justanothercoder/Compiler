#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr) { }

ReturnNode::~ReturnNode() { delete expr; }

void ReturnNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> _expr)
{
    expr->gen(template_sym, _expr);

	auto copy_constr = TypeHelper::getCopyConstructor(TypeHelper::removeReference(expr->getType()));

	CodeGen::genCopy(copy_constr, getScope()->get_valloc()->getAddressForLocal(), expr->getType());

    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
}

void ReturnNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> _expr)
{
	auto scope = getScope();
	while ( scope != nullptr && dynamic_cast<FunctionSymbol*>(scope) == nullptr )
		scope = scope->getEnclosingScope();

	if ( scope == nullptr )
		throw SemanticError("return is not a in a function");

    expr->check(template_sym, _expr);
}

void ReturnNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> _expr) { expr->define(template_sym, _expr); }

AST* ReturnNode::copyTree() const { return new ReturnNode(static_cast<ExprNode*>(expr->copyTree())); }

vector<AST*> ReturnNode::getChildren() const { return {expr}; }
