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

    if ( sym == nullptr )
	throw SemanticError("No such symbol " + name);

    if ( dynamic_cast<TypedSymbol*>(sym) == nullptr )
	throw SemanticError(name + " is not a variable");

    variable = static_cast<TypedSymbol*>(sym);   
}

void VariableNode::gen()
{
    Type *var_type = variable->getType();
    
    if ( dynamic_cast<ReferenceType*>(var_type) )
    {
	CodeGen::emit("mov eax, [rbp - " + std::to_string(scope->getAddress(dynamic_cast<VariableSymbol*>(variable))) + "]");
    }    
    else if ( dynamic_cast<OverloadedFunctionType*>(var_type) )
    {
	OverloadedFunctionType *func_type = static_cast<OverloadedFunctionType*>(var_type);
	
	if ( func_type->overloads.size() > 1 )
	{
	    Type *hint_type = scope->getTypeHint(this);
	    if ( hint_type == nullptr )
		throw SemanticError("multiple overloads of " + name);
	   
	    variable = dynamic_cast<TypedSymbol*>(func_type->symbols[static_cast<FunctionType*>(hint_type)]);
	}
	else
	{
	    variable = func_type->symbols.begin()->second;
	}
	CodeGen::emit("lea eax, [" + static_cast<FunctionSymbol*>(variable)->getTypedName() + "]");
    }
    else
    {
	CodeGen::emit("lea eax, [rbp - " + std::to_string(scope->getAddress(dynamic_cast<VariableSymbol*>(variable))) + "]");
    }
}

bool VariableNode::isLeftValue()
{
    return true;
}

void VariableNode::build_scope()
{
    
}
