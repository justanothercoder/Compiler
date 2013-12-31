#include "builtintypesymbol.hpp"

BuiltInTypeSymbol::BuiltInTypeSymbol(string name, int size) : Symbol(name), size(size)
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
