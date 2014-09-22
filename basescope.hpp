#ifndef _BASESCOPE_HPP_
#define _BASESCOPE_HPP_

#include <map>

#include "scope.hpp"

class ExprNode;

class BaseScope : public Scope
{
public:

    friend class VariableSymbolDefine;
    friend class FunctionSymbolDefine;
    friend class SymbolDefine;
    
	~BaseScope() override;

    Symbol* resolve(std::string name) const override;

public:
	std::map<std::string, Symbol*> table;
};

#endif
