#include "statementnode.hpp"

StatementNode::StatementNode(const vector<AST*>& statements) : statements(statements)
{
    
}

StatementNode::~StatementNode()
{
    for ( auto i : statements )
	delete i;
}

void StatementNode::process()
{
    for ( auto i : statements )
	i->process();
}

void StatementNode::setStatements(const vector<AST*>& statements)
{
    this->statements = statements;
}
