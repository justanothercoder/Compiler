#include "statementnode.hpp"

StatementNode::StatementNode(const vector<AST*>& statements) : statements(statements)
{

}

StatementNode::~StatementNode()
{
    for ( auto i : statements )
	delete i;
}

void StatementNode::build_scope()
{
    for ( auto i : this->statements )
    {
	i->scope = scope;
	i->build_scope();
    }
}

void StatementNode::process()
{
    for ( auto i : statements )
	i->process();
}
