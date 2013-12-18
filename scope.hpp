#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include "symbol.hpp"

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* getEnclosingScope() = 0;
    virtual Symbol* resolve(string name) = 0;
    virtual void define(Symbol *sym) = 0;
    
protected:
    
    
};

#endif
