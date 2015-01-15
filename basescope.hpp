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

    const Symbol* resolve(std::string name) const override;
    const Symbol* resolveHere(std::string name) const override;

public:
    std::map<std::string, std::shared_ptr<const Symbol> > table;
};

#endif
