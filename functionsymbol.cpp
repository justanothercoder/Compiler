#include "functionsymbol.hpp"
#include "scopevisitor.hpp"
#include "functionsymboldefine.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"

#include "logger.hpp"

FunctionSymbol::FunctionSymbol(const std::string& name
                             , FunctionType _type
                             , FunctionScope* scope
                             , FunctionTraits traits) : name        (name)
                                                      , traits      (traits)
                                                      , _type       (_type)
                                                      , scope       (scope)
{

}

std::string FunctionSymbol::getTypedName() const
{
    auto res = (traits.is_operator ? Comp::config().getCodeOperatorName(name) : name);

    for ( auto param_type : _type.typeInfo().params() )
        res += "_" + param_type.getName();

    return res;
}

std::string FunctionSymbol::getScopedTypedName() const
{
    auto res = scope -> getScopeName(); 

    auto pt = _type.typeInfo().params();

    for ( auto param_type : pt ) 
        res += "_" + param_type.getName();

    return res;
}

bool FunctionSymbol::isOperator() const { return traits.is_operator; }
bool FunctionSymbol::isMethod() const { return traits.is_method; }
bool FunctionSymbol::isConstructor() const { return traits.is_constructor; }

FunctionTraits FunctionSymbol::getTraits() const { return traits; } 
SymbolType FunctionSymbol::getSymbolType() const { return SymbolType::FUNCTION; }

std::string FunctionSymbol::getName() const { return name; }
FunctionType FunctionSymbol::type() const { return _type; } 
Scope* FunctionSymbol::getScope() const { return scope; }

std::unique_ptr<DefineSymbolVisitor> FunctionSymbol::defineSymbolVisitor() const { return std::make_unique<FunctionSymbolDefine>(); }
