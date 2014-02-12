#include "statementnode.hpp"

StatementNode::StatementNode(const vector<AST*>& statements) : statements(statements) { }

StatementNode::~StatementNode()
{
	for ( auto i : statements )
		delete i;
}

void StatementNode::build_scope()
{
	for ( auto i : this->statements )
	{
		i->setScope(getScope());
		i->build_scope();
	}
}

void StatementNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	for ( auto i : statements )
		i->define(template_sym, expr);
}

void StatementNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	for ( auto i : statements )
		i->check(template_sym, expr);
}

AST* StatementNode::copyTree() const
{
	vector<AST*> stats(statements.size());

	std::transform(std::begin(statements), std::end(statements), std::begin(stats), [&](AST *t) { return t->copyTree(); });

	return new StatementNode(stats);
}

void StatementNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	for ( auto i : statements )
		i->gen(template_sym, expr);
}
