#include "variablesymbol.hpp"

VariableSymbol::VariableSymbol(string name, Type *type, bool is_param) : name(name), type(type), is_param(is_param)
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
