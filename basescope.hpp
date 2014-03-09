#ifndef _BASESCOPE_HPP_
#define _BASESCOPE_HPP_

#include <map>

#include "scope.hpp"
#include "variablesymbol.hpp"

using std::map;

class ExprNode;

class BaseScope : public Scope
{
public:

    friend class VariableSymbolDefine;
    friend class FunctionSymbolDefine;
    friend class SymbolDefine;
    
    virtual Symbol* resolve(string name) const;

public:
    map<string, Symbol*> table;
};

#endif
