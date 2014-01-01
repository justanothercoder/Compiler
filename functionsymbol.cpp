#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name, FunctionType *function_type, Scope *enclosing_scope) : TypedSymbol(name), function_type(function_type), enclosing_scope(enclosing_scope)
{
    
}

Scope* FunctionSymbol::getEnclosingScope()
{
    return enclosing_scope;
}

Symbol* FunctionSymbol::resolve(string name)
{
    auto it = members.find(name);
    if ( it == std::end(members) )
    {
	if ( getEnclosingScope() )
	    return getEnclosingScope()->resolve(name);
	return nullptr;
    }
    return it->second;    
}

void FunctionSymbol::define(Symbol *sym)
{
    if ( members.find(name) == std::end(members) )
	members[sym->getName()] = sym;
}

string FunctionSymbol::getName()
{
    return name;
}

Type* FunctionSymbol::getType()
{
    return function_type;
}
