#include "variablesymbol.hpp"

VariableSymbol::VariableSymbol(string name, Type *type) : TypedSymbol(name), type(type)
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
