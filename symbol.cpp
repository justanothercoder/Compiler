#include "symbol.hpp"

Symbol::Symbol(string name) : name(name)
{
    
}

Symbol::~Symbol()
{
    
}

string Symbol::getName()
{
    return name;
}

void Symbol::setName(string s)
{
    name = s;
}
