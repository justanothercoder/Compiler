#ifndef _BASESCOPE_HPP_
#define _BASESCOPE_HPP_

#include <map>

#include "scope.hpp"

using std::map;

class ExprNode;

class BaseScope : public Scope
{
public:

    BaseScope();

    virtual Symbol* resolve(string name) const;

    virtual int getAddress(VariableSymbol *sym) const;

    virtual int getScopeSize() const;
    virtual int getScopeAddress() const;

protected:

    map<string, Symbol*> table;
    map<VariableSymbol*, int> addresses;

    int scope_size, scope_address;

};

#endif