#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include "symbol.hpp"
#include "type.hpp"
#include "exprnode.hpp"

class VariableSymbol;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* getEnclosingScope() const = 0;
    virtual Symbol* resolve(string name) const = 0;

    virtual void define(Symbol *sym) = 0;

    virtual int getAddress(VariableSymbol* sym) const = 0;

    virtual int getScopeAddress() const = 0;
    virtual int getScopeSize() const = 0;

    virtual string getScopeName() const = 0;
};

#endif
