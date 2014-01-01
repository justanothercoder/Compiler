#include "assignmentnode.hpp"

AssignmentNode::AssignmentNode(ExprNode *lhs, ExprNode *rhs) : lhs(lhs), rhs(rhs)
{
    
}

void AssignmentNode::build_scope()
{
    
}

void AssignmentNode::define()
{
    
}

void AssignmentNode::check()
{
    lhs->check();
    rhs->check();

    if ( !lhs->isLeftValue() )
	throw;

    if ( lhs->getType() != rhs->getType() )
	throw;    
}

void AssignmentNode::gen()
{

}
