#include "symboltable.hpp"
#include "builtintypesymbol.hpp"
#include "functionalsymbol.hpp"
#include "varsymbol.hpp"
#include "aliassymbol.hpp"
#include "typesymbol.hpp"
#include "templatesymbol.hpp"
#include "modulesymbol.hpp"

void SymbolTable::define(std::unique_ptr<BuiltInTypeSymbol> builtin)
{
    auto name = builtin -> getName();
    table.emplace(name, SymbolEntry{std::move(builtin), SymbolType::BuiltIn});
}

void SymbolTable::define(std::unique_ptr<FunctionalSymbol> func)
{
    auto name = func -> getName();
    table.emplace(name, SymbolEntry{std::move(func), SymbolType::Functional});
}

void SymbolTable::define(std::unique_ptr<VarSymbol> var)
{
    auto name = var -> getName();
    table.emplace(name, SymbolEntry{std::move(var), SymbolType::Var});
}

void SymbolTable::define(std::unique_ptr<AliasSymbol> alias)
{
    auto name = alias -> getName();
    table.emplace(name, SymbolEntry{std::move(alias), SymbolType::Alias});
}

void SymbolTable::define(std::unique_ptr<TemplateSymbol> templ)
{
    auto name = templ -> getName();
    table.emplace(name, SymbolEntry{std::move(templ), SymbolType::Template});
}

void SymbolTable::define(std::unique_ptr<TypeSymbol> type)
{
    auto name = type -> getName();
    table.emplace(name, SymbolEntry{std::move(type), SymbolType::Type});
}

void SymbolTable::define(std::shared_ptr<ModuleSymbol> module)
{
    auto name = module -> getName();
    table.emplace(name, SymbolEntry{std::make_unique<AliasSymbol>(module.get()), SymbolType::Module});
} 

TypeSymbol* SymbolTable::resolveType(const std::string& name) const
{
    auto it = table.find(name);
    
    if ( it == std::end(table) )
        return nullptr;

    if ( it -> second.type() != SymbolType::Type )
        throw SemanticError("Type not found");

    return static_cast<TypeSymbol*>(it -> second.symbol());
}

std::vector<FunctionalSymbol*> SymbolTable::resolveFunction(const std::string& name) const
{
    auto range = table.equal_range(name);
    
    auto res = std::vector<FunctionalSymbol*>{ };
    for ( auto it = range.first; it != range.second; ++it )
    {
        assert(it -> second.type() == SymbolType::Functional);
        res.emplace_back(static_cast<FunctionalSymbol*>(it -> second.symbol()));
    }

    return res;
}

VarSymbol* SymbolTable::resolveVariable(const std::string& name) const
{
    auto it = table.find(name);

    if ( it == std::end(table) )
        return nullptr;

    if ( it -> second.type() != SymbolType::Var )
        throw SemanticError("Type not found");

    return static_cast<VarSymbol*>(it -> second.symbol());
}

Symbol* SymbolTable::resolve(const std::string& name) const
{
    auto it = table.find(name);
    if ( it == std::end(table) )
        return nullptr;
    return it -> second.symbol();
}
    
std::vector<VarSymbol*> SymbolTable::getVars() const 
{
    std::vector<VarSymbol*> vars;

    for ( const auto& entry : table )
    {
        if ( entry.second.type() == SymbolType::Var )
            vars.emplace_back(static_cast<VarSymbol*>(entry.second.symbol()));
    }

    return vars;
}

std::vector<FunctionalSymbol*> SymbolTable::getFunctions() const
{
    std::vector<FunctionalSymbol*> funcs;

    for ( const auto& entry : table )
    {
        if ( entry.second.type() == SymbolType::Functional )
            funcs.emplace_back(static_cast<FunctionalSymbol*>(entry.second.symbol()));
    }

    return funcs;
}
    
std::vector<Symbol*> SymbolTable::allSymbols() const
{
    std::vector<Symbol*> symbols;

    for ( const auto& entry : table )
        symbols.emplace_back(entry.second.symbol());

    return symbols;
}
