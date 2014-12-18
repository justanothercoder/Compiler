#include "functionnode.hpp"

FunctionNode::FunctionNode(std::string name) : name(name), function(nullptr)
{

}    

AST* FunctionNode::copyTree() const 
{
    return new FunctionNode(name);
}

VariableType FunctionNode::getType() const 
{
    return function;   
}

bool FunctionNode::isLeftValue() const 
{
    return false;
}

bool FunctionNode::isCompileTimeExpr() const 
{
    return false;
}

boost::optional<int> FunctionNode::getCompileTimeValue() const 
{
    return boost::none;
}

std::string FunctionNode::toString() const 
{
    return name;
}

void FunctionNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(this);
}

