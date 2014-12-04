#include "modulenode.hpp"

ModuleNode::ModuleNode(std::string name) : name(name), module(nullptr)
{

}

AST* ModuleNode::copyTree() const 
{
    return new ModuleNode(name);
}

const Type* ModuleNode::getType() const 
{
    return nullptr;
}

bool ModuleNode::isLeftValue() const 
{
    return false;
}

bool ModuleNode::isCompileTimeExpr() const
{
    return false;    
}

boost::optional<int> ModuleNode::getCompileTimeValue() const 
{
    return boost::none;
}

std::string ModuleNode::toString() const 
{
    return name;
}

void ModuleNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(this);
}

