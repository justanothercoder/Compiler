#ifndef _SYMBOLTABLE_HPP_
#define _SYMBOLTABLE_HPP_

#include <vector>
#include <map>
#include "symbolentry.hpp"

class FunctionalSymbol;
class BuiltInTypeSymbol;
class VarSymbol;
class AliasSymbol;
class TemplateSymbol;
class TypeSymbol;
class ModuleSymbol;

class SymbolTable
{
public:

    void define(std::unique_ptr<BuiltInTypeSymbol> builtin);
    void define(std::unique_ptr<FunctionalSymbol> func);
    void define(std::unique_ptr<VarSymbol> var);
    void define(std::unique_ptr<AliasSymbol> type);
    void define(std::unique_ptr<TemplateSymbol> templ);
    void define(std::unique_ptr<TypeSymbol> type);

    void define(std::shared_ptr<ModuleSymbol> module); 

    TypeSymbol* resolveType(const std::string& name) const;
    std::vector<FunctionalSymbol*> resolveFunction(const std::string& name) const;
    VarSymbol* resolveVariable(const std::string& name) const;

    Symbol* resolve(const std::string& name) const;

    std::vector<VarSymbol*> getVars() const;
    std::vector<FunctionalSymbol*> getFunctions() const;

    std::vector<Symbol*> allSymbols() const;

private:

    std::multimap<std::string, SymbolEntry> table;
};

#endif
