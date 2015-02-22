#include "scope.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include "functionalsymbol.hpp"
#include "typesymbol.hpp"
#include "varsymbol.hpp"
#include "functiontypeinfo.hpp"

#include "logger.hpp"

Scope::~Scope() { }
bool Scope::isUnsafeBlock() const { return false; }

TypeSymbol* Scope::resolveType(const std::string& name) const
{
    auto scope = this;
    Symbol *sym = nullptr;
    while ( !sym || !sym -> isType() )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    return static_cast<TypeSymbol*>(sym);
}

FunctionalSymbol* Scope::resolveFunction(const std::string& name, const FunctionTypeInfo& info) const
{
    auto scope = this;
    Symbol* sym = nullptr;
    while ( !sym || !sym -> isFunction() || !static_cast<FunctionalSymbol*>(sym) -> isCompatibleWith(info) )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    return static_cast<FunctionalSymbol*>(sym);
}

VarSymbol* Scope::resolveVariable(const std::string& name) const
{
    auto scope = this;
    Symbol* sym = nullptr;
    while ( !sym || !sym -> isVariable() )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    return static_cast<VarSymbol*>(sym);
}

