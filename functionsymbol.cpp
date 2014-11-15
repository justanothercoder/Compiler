#include "functionsymbol.hpp"
#include "scopevisitor.hpp"
#include "functionsymboldefine.hpp"
#include "globalconfig.hpp"

FunctionSymbol::FunctionSymbol(std::string name
                               , const FunctionType *type
                               , FunctionScope *scope
                               , FunctionTraits traits
                              ) 
//: FunctionScope(enclosing_scope -> getScopeName() 
  //                               + "_" + (traits.is_operator ? GlobalConfig::getCodeOperatorName(name) : name), enclosing_scope,
//                                 traits.is_constructor)
                                : is_constexpr(false)
                                , is_used(false)
                                , name(name)
                                , traits(traits)
                                , type(type)
                                , scope(scope)
{

}

std::string FunctionSymbol::getTypedName() const
{
    auto res = (traits.is_operator ? GlobalConfig::getCodeOperatorName(name) : name);

    for ( auto type : type -> getTypeInfo().params_types )
        res += "_" + type -> getName();

    return res;
}

std::string FunctionSymbol::getScopedTypedName() const
{
    auto res = scope -> getScopeName(); //scope_name;

    auto& pt = type -> getTypeInfo().params_types;

    for ( auto type : pt )
        res += "_" + type -> getName();

    return res;
}

bool FunctionSymbol::isOperator() const
{
    return traits.is_operator;
}

bool FunctionSymbol::isMethod() const
{
    return traits.is_method;
}

bool FunctionSymbol::isConstructor() const
{
    return traits.is_constructor;
}

std::string FunctionSymbol::getName() const
{
    return name;
}

FunctionTraits FunctionSymbol::getTraits() const
{
    return traits;
}

SymbolType FunctionSymbol::getSymbolType() const
{
    return SymbolType::FUNCTION;
}

ScopeVisitor& FunctionSymbol::getScopeVisitor()
{
    return *(new FunctionSymbolDefine(this));
}

  
const FunctionType* FunctionSymbol::getType() const
{
    return type;
}

Scope* FunctionSymbol::getScope() const
{
    return scope;
}
