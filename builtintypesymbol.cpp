#include "builtintypesymbol.hpp"

BuiltInTypeSymbol::BuiltInTypeSymbol(string name, int size) : name(name), size(size)
{
    
}
    
string BuiltInTypeSymbol::getName()
{
    return name;
}

int BuiltInTypeSymbol::getSize()
{
    return size;
}

Scope* BuiltInTypeSymbol::getScope()
{
    return symbol_scope;
}

void BuiltInTypeSymbol::setScope(Scope *scope)
{
    symbol_scope = scope;
}
