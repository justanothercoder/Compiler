#include "variablenode.hpp"

VariableNode::VariableNode(string name) : name(name)
{
    
}

Type* VariableNode::getType()
{
    return variable->getType();
}

void VariableNode::check()
{
    Symbol *sym = scope->resolve(name);

    if ( sym == nullptr || dynamic_cast<TypedSymbol*>(sym) == nullptr )
	throw;

    variable = static_cast<TypedSymbol*>(sym);   
}

void VariableNode::gen()
{
    if ( dynamic_cast<FunctionType*>(variable->getType()) != nullptr )
	CodeGen::emit("lea eax, [" + name + "]");
}

bool VariableNode::isLeftValue()
{
    return true;
}

void VariableNode::build_scope()
{
    
}
