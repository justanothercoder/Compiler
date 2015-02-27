#include "scope.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include "functionalsymbol.hpp"
#include "typesymbol.hpp"
#include "varsymbol.hpp"
#include "templatesymbol.hpp"
#include "functiontypeinfo.hpp"
#include "templatefunctionsymbol.hpp"

#include "logger.hpp"

Scope::~Scope() { }
bool Scope::isUnsafeBlock() const { return false; }

TypeSymbol* Scope::resolveType(const std::string& name) const
{
    auto scope = this;
    Symbol *sym = nullptr;
    while ( scope && (!sym || !sym -> isType()) )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    if ( !sym || !sym -> isType() )
        return nullptr;

    return static_cast<TypeSymbol*>(sym);
}

FunctionalSymbol* Scope::resolveFunction(const std::string& name, const FunctionTypeInfo& info) const
{
    auto scope = this;
    Symbol* sym = nullptr;
    while ( scope && (!sym || !sym -> isFunction() || !static_cast<FunctionalSymbol*>(sym) -> isCompatibleWith(info)) )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    if ( !sym || !sym -> isFunction() || !static_cast<FunctionalSymbol*>(sym) -> isCompatibleWith(info) )
        return nullptr;

    return static_cast<FunctionalSymbol*>(sym);
}

VarSymbol* Scope::resolveVariable(const std::string& name) const
{
    auto scope = this;
    Symbol* sym = nullptr;
    while ( scope && (!sym || !sym -> isVariable()) )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    if ( !sym || !sym -> isVariable() )
        return nullptr;

    return static_cast<VarSymbol*>(sym);
}
    
Symbol* Scope::resolveTemplate(const std::string& name, const TemplateArguments& args) const
{
    auto scope = this;
    Symbol* sym = nullptr;
    while ( scope && (!sym || !dynamic_cast<TemplateSymbol*>(sym) || !static_cast<TemplateSymbol*>(sym) -> canBeSpecializedWith(args)) )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    if ( !sym || !dynamic_cast<TemplateSymbol*>(sym) || !static_cast<TemplateSymbol*>(sym) -> canBeSpecializedWith(args) )
        return nullptr;

    return sym;
}

FunctionalSymbol* Scope::resolveTemplateFunction(const std::string& name, const TemplateArguments& args, const FunctionTypeInfo& info) const
{
    auto scope = this;
    Symbol* sym = nullptr;
    while ( scope && (!sym || !dynamic_cast<TemplateFunctionSymbol*>(sym) || !static_cast<TemplateFunctionSymbol*>(sym) -> canBeSpecializedWith(args, info)) )
    {
        sym = scope -> resolve(name);
        scope = scope -> enclosingScope();
    }

    if ( !sym || !dynamic_cast<TemplateFunctionSymbol*>(sym) || !static_cast<TemplateFunctionSymbol*>(sym) -> canBeSpecializedWith(args, info) )
        return nullptr;

    return static_cast<TemplateFunctionSymbol*>(sym) -> overloadOfTemplateFunction(info, args);;
}
