#ifndef _BUILTINTYPESYMBOL_HPP_
#define _BUILTINTYPESYMBOL_HPP_

#include "type.hpp"
#include "symbol.hpp"

class BuiltInTypeSymbol : public Symbol, public Type
{
public:

    BuiltInTypeSymbol(string name, int size);
    
    virtual string getName();
    virtual int getSize();    

private:

    int size;
};

#endif
