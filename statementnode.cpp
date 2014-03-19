#include "statementnode.hpp"

StatementNode::StatementNode(vector<AST*> statements) : statements(statements) { }

StatementNode::~StatementNode()
{
	for ( auto i : statements )
		delete i;
}

void StatementNode::define(const TemplateInfo& template_info)
{
	for ( auto i : statements )
		i->define(template_info);
}

void StatementNode::check(const TemplateInfo& template_info)
{
	for ( auto i : statements )
		i->check(template_info);
}

AST* StatementNode::copyTree() const
{
	vector<AST*> stats(statements.size());

	std::transform(std::begin(statements), std::end(statements), std::begin(stats), [&](AST *t) { return t->copyTree(); });

	return new StatementNode(stats);
}

void StatementNode::gen(const TemplateInfo& template_info)
{
	for ( auto i : statements )
		i->gen(template_info);
}

vector<AST*> StatementNode::getChildren() const { return statements; }
