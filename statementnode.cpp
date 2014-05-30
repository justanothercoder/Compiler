#include "statementnode.hpp"

StatementNode::StatementNode(vector<AST*> statements) : statements(statements), code_obj() { }

StatementNode::~StatementNode()
{
	for ( auto i : statements )
		delete i;
}

void StatementNode::define()
{
	for ( auto i : statements )
		i->define();
}

void StatementNode::check()
{
	for ( auto i : statements )
		i->check();
}

AST* StatementNode::copyTree() const
{
	vector<AST*> stats(statements.size());

	std::transform(std::begin(statements), std::end(statements), std::begin(stats), [&](AST *t) { return t->copyTree(); });

	return new StatementNode(stats);
}

CodeObject& StatementNode::gen()
{
	for ( auto i : statements )
		code_obj.emit(i->gen().getCode());
	return code_obj;
}

vector<AST*> StatementNode::getChildren() const { return statements; }
