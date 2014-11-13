#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>
#include <boost/optional.hpp>

#include "functionscope.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"
#include "symbol.hpp"
#include "functiontype.hpp"
#include "globaltable.hpp"

class VariableSymbol;

class FunctionSymbol : public Symbol
{
public:

    FunctionSymbol(std::string name, const FunctionType *type, FunctionScope *scope, FunctionTraits traits);

    std::string getTypedName() const;
    std::string getScopedTypedName() const;

    const FunctionType* getType() const;

    Scope* getScope() const;

    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

    std::string getName() const override;
    SymbolType getSymbolType() const override;

    FunctionTraits getTraits() const;

    bool is_constexpr;

    mutable bool is_used;

    ScopeVisitor& getScopeVisitor() override;

private:

    std::string name;

    FunctionTraits traits;
    const FunctionType *type;
    FunctionScope *scope;
};

#endif
