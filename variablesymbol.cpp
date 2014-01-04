#include "variablesymbol.hpp"

VariableSymbol::VariableSymbol(string name, Type *type) : name(name), type(type)
{
    
}

Type* VariableSymbol::getType()
{
    return type;
}

void VariableSymbol::setType(Type *t)
{
    type = t;
}

Scope* VariableSymbol::getScope()
{
    return symbol_scope;
}

void VariableSymbol::setScope(Scope *scope)
{
    symbol_scope = scope;
}

string VariableSymbol::getName()
{
    return name;
}

bool VariableSymbol::isParam()
{
    return is_param;
}
