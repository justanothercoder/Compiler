#include "globalscope.hpp"

GlobalScope::GlobalScope()
{
    scope_size = 0;
}

Scope* GlobalScope::getEnclosingScope()
{
    return nullptr;
}

Symbol* GlobalScope::resolve(string name)
{
    auto it = table.find(name);
    if ( it == std::end(table) )
	return nullptr;
    return it->second;
}

void GlobalScope::define(Symbol *sym)
{
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	if ( table[sym->getName()] == nullptr )
	    table[sym->getName()] = new OverloadedFunctionSymbol(sym->getName(), OverloadedFunctionTypeInfo({ }));

	OverloadedFunctionSymbol* ov_func = dynamic_cast<OverloadedFunctionSymbol*>(table[sym->getName()]);
	
	if ( ov_func == nullptr )
	    throw SemanticError(sym->getName() + " is already defined as not function");
	
	OverloadedFunctionTypeInfo type_info = ov_func->getTypeInfo();
	
	FunctionTypeInfo func_type_info = static_cast<FunctionSymbol*>(sym)->getTypeInfo();
	type_info.overloads.insert(func_type_info);
	type_info.symbols[func_type_info] = static_cast<FunctionSymbol*>(sym);
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	table[sym->getName()] = sym;
	addresses[static_cast<VariableSymbol*>(sym)] = (scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize());
    }
    else
	table[sym->getName()] = sym;
}
    
Type* GlobalScope::getTypeHint(ExprNode *expr)
{
    return type_hints[expr];
}

void GlobalScope::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}
    
int GlobalScope::getAddress(VariableSymbol *sym)
{
    auto it = addresses.find(sym);

    if ( it == std::end(addresses) )
	throw SemanticError("No such symbol " + sym->getName());

    return it->second;   
}

int GlobalScope::getScopeAddress()
{
    return 0;
}

int GlobalScope::getScopeSize()
{
    return scope_size;
}

string GlobalScope::getScopeName()
{
    return "";
}
