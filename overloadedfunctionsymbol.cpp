#include "overloadedfunctionsymbol.hpp"

OverloadedFunctionSymbol::OverloadedFunctionSymbol(string name, OverloadedFunctionTypeInfo type_info) : name(name), type_info(type_info)
{
    
}

string OverloadedFunctionSymbol::getName()
{
    return name;   
}

int OverloadedFunctionSymbol::getSize()
{
    return sizeof(int*);
}

Scope* OverloadedFunctionSymbol::getScope()
{
    return symbol_scope;
}

void OverloadedFunctionSymbol::setScope(Scope *scope)
{
    symbol_scope = scope;
}

OverloadedFunctionTypeInfo OverloadedFunctionSymbol::getTypeInfo()
{
    return type_info;
}
