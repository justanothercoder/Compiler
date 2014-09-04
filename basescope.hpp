#ifndef _BASESCOPE_HPP_
#define _BASESCOPE_HPP_

#include <map>

#include "scope.hpp"
#include "variablesymbol.hpp"

class ExprNode;

class BaseScope : public Scope
{
public:

    friend class VariableSymbolDefine;
    friend class FunctionSymbolDefine;
    friend class SymbolDefine;
    
    virtual Symbol* resolve(std::string name) const;

public:
	std::map<std::string, Symbol*> table;
};

#endif
