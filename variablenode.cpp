#include "variablenode.hpp"

VariableNode::VariableNode(string name) : name(name)
{
    
}

void VariableNode::build_scope()
{
    
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
	if ( variable->isField() )
	{
	    VariableSymbol *sym = static_cast<VariableSymbol*>(scope->resolve("this"));

	    Scope *struc_scope = static_cast<StructSymbol*>(sym->getType());
	    
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(scope->getAddress(sym)) + "]");
	    CodeGen::emit("mov rax, [rax - " + std::to_string(struc_scope->getAddress(variable)) + "]");
	}
	else
	{
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(scope->getAddress(dynamic_cast<VariableSymbol*>(variable))) + "]");
	}
    }    
    else if ( dynamic_cast<OverloadedFunctionSymbol*>(var_type) )
    {
	OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(var_type);
	
	OverloadedFunctionTypeInfo ov_func_type_info = ov_func->getTypeInfo();
	
	if ( ov_func_type_info.overloads.size() > 1 )
	{
	    FunctionSymbol *hint_type = static_cast<FunctionSymbol*>(scope->getTypeHint(this));
	    if ( hint_type == nullptr )
		throw SemanticError("multiple overloads of " + name);
	   
	    variable = dynamic_cast<VariableSymbol*>(ov_func_type_info.symbols[hint_type->getTypeInfo()]);
	}
	else
	{
	    variable = new VariableSymbol(ov_func->getName(), ov_func_type_info.symbols.begin()->second);
	}

	CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(variable->getType())->getScopedTypedName() + "]");
    }
    else
    {
	if ( variable->isField() )
	{
	    Symbol *_this = scope->resolve("this");

	    if ( dynamic_cast<VariableSymbol*>(_this) == nullptr )
		throw SemanticError("internal error");
	    
	    VariableSymbol *sym = static_cast<VariableSymbol*>(_this);

	    Scope *struc_scope = static_cast<StructSymbol*>(static_cast<ReferenceType*>(sym->getType())->getReferredType());
	    
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(scope->getAddress(sym)) + "]");
	    CodeGen::emit("lea rax, [rax - " + std::to_string(struc_scope->getAddress(variable)) + "]");
	}
	else
	{
	    CodeGen::emit("lea rax, [rbp - " + std::to_string(scope->getAddress(dynamic_cast<VariableSymbol*>(variable))) + "]");
	}
    }
}


Type* VariableNode::getType() const
{
    return TypeHelper::getReferenceType(variable->getType());
}

bool VariableNode::isLeftValue() const
{
    return true;
}
