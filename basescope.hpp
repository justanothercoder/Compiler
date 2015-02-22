#ifndef _BASESCOPE_HPP_
#define _BASESCOPE_HPP_

#include <map>
#include "scope.hpp"
#include "symboltable.hpp"

class ExprNode;

class BaseScope : public Scope
{
public:

    ~BaseScope() override;

    Symbol* resolve(const std::string& name) const override;
    Symbol* resolveHere(const std::string& name) const override;
    
    void define(std::unique_ptr<BuiltInTypeSymbol> builtin) override;
    void define(std::unique_ptr<FunctionalSymbol> func) override;
    void define(std::unique_ptr<VarSymbol> var) override;
    void define(std::unique_ptr<AliasSymbol> type) override;
    void define(std::unique_ptr<TemplateSymbol> templ) override;
    void define(std::unique_ptr<TypeSymbol> type) override;

    void define(std::shared_ptr<ModuleSymbol> module) override; 
    
    std::vector<VarSymbol*> getVars() const;
    std::vector<FunctionalSymbol*> getFunctions() const;
    std::vector<Symbol*> allSymbols() const;
    
    FunctionalSymbol* resolveFunction(const std::string& name, const FunctionTypeInfo& info) const override;

public:
    SymbolTable table;
};

#endif
