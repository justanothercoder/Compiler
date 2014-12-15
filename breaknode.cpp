#include "breaknode.hpp"

BreakNode::BreakNode()
{

}

AST* BreakNode::copyTree() const 
{
    return new BreakNode();
}

std::string BreakNode::toString() const 
{
    return "break";
}

void BreakNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(this);
}
