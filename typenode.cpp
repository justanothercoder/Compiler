#include "typenode.hpp"

TypeNode::TypeNode(std::string name) : name(name), type_symbol(nullptr)
{

}

AST* TypeNode::copyTree() const
{
    return new TypeNode(name);
}

VariableType TypeNode::getType() const 
{
    return nullptr;
}

bool TypeNode::isLeftValue() const
{
    return false;
}

bool TypeNode::isCompileTimeExpr() const
{
    return false;
}

boost::optional<int> TypeNode::getCompileTimeValue() const
{
    return boost::none;
}

std::string TypeNode::toString() const 
{
    return name;
}

void TypeNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
