#ifndef _SCOPEDSYMBOL_HPP_
#define _SCOPEDSYMBOL_HPP_

#include "symbol.hpp"

class Scope;

class ScopedSymbol : public Symbol
{
public:

    virtual Scope* innerScope() const = 0;
};

#endif
