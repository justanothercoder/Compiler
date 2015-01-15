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
class FunctionDeclarationNode;

class FunctionSymbol : public Symbol
{
public:

    FunctionSymbol(const std::string& name, FunctionType type, FunctionScope* scope, FunctionTraits traits);

    std::string getTypedName() const;
    std::string getScopedTypedName() const;

    FunctionType type() const;

    Scope* getScope() const;

    bool isOperator() const;
    bool isMethod() const;
    bool isConstructor() const;

    std::string getName() const override;
    SymbolType getSymbolType() const override;
    FunctionTraits getTraits() const;

    bool is_constexpr = false;
    mutable bool is_used = false;

    std::unique_ptr<DefineSymbolVisitor> defineSymbolVisitor() const override;

    FunctionDeclarationNode* function_decl = nullptr;
private:

    std::string name;

    FunctionTraits traits;
    FunctionType _type;
    FunctionScope* scope;
};

#endif
