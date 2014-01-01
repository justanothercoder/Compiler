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
    if ( dynamic_cast<OverloadedFunctionType*>(variable->getType()) != nullptr )
    {
	if ( static_cast<OverloadedFunctionType*>(variable->getType())->overloads.size() > 1 )
	{
	    if ( scope->getTypeHint(this) == nullptr )
		throw;
	   
	    variable = static_cast<OverloadedFunctionType*>(variable->getType())->symbols[static_cast<FunctionType*>(scope->getTypeHint(this))];
	}
	else
	{
	    variable = static_cast<OverloadedFunctionType*>(variable->getType())->symbols.begin()->second;
	}
	CodeGen::emit("lea eax, [" + static_cast<FunctionSymbol*>(variable)->getTypedName() + "]");
    }
}

bool VariableNode::isLeftValue()
{
    return true;
}

void VariableNode::build_scope()
{
    
}
