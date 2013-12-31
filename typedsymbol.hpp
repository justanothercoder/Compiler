#ifndef _TYPEDSYMBOL_HPP_
#define _TYPEDSYMBOL_HPP_

#include "symbol.hpp"
#include "type.hpp"

class TypedSymbol : public Symbol
{
public:

    TypedSymbol(string name);
    
    virtual Type* getType() = 0;
};

#endif
