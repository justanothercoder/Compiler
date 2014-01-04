#include "variablenode.hpp"

VariableNode::VariableNode(string name) : name(name)
{
    
}

Type* VariableNode::getType()
{
    return TypeHelper::getReferenceType(variable->getType());
}

void VariableNode::check()
{
    Symbol *sym = scope->resolve(name);

    if ( sym == nullptr )
	throw SemanticError("No such symbol " + name);

    if ( dynamic_cast<VariableSymbol*>(sym) == nullptr )
	throw SemanticError("'" + name + "' is not a variable.");

    variable = static_cast<VariableSymbol*>(sym);
}

void VariableNode::gen()
{
    Type *var_type = variable->getType();
    
    if ( dynamic_cast<ReferenceType*>(var_type) )
    {
	CodeGen::emit("mov eax, [rbp - " + std::to_string(scope->getAddress(dynamic_cast<VariableSymbol*>(variable))) + "]");
    }    
    else if ( dynamic_cast<OverloadedFunctionSymbol*>(variable->getType()) )
    {
	OverloadedFunctionSymbol* ov_func = dynamic_cast<OverloadedFunctionSymbol*>(variable->getType());
	
	OverloadedFunctionTypeInfo ov_func_type_info = ov_func->getTypeInfo();
	
	if ( ov_func_type_info.overloads.size() > 1 )
	{
	    Type *hint_type = scope->getTypeHint(this);
	    if ( hint_type == nullptr )
		throw SemanticError("multiple overloads of " + name);
	   
	    variable = dynamic_cast<VariableSymbol*>(ov_func_type_info.symbols[static_cast<FunctionSymbol*>(hint_type)->getTypeInfo()]);
	}
	else
	{
	    variable = new VariableSymbol(ov_func->getName(), ov_func_type_info.symbols.begin()->second);
	}

	CodeGen::emit("lea eax, [" + static_cast<FunctionSymbol*>(variable->getType())->getTypedName() + "]");
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
