#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <set>
#include <vector>
#include <string>
#include <memory>

class Type;
class Symbol;
class FunctionalType;
struct TemplateInfo;

class TemplateSymbol;
class FunctionalSymbol;
class VarSymbol;
class TypeSymbol;
class BuiltInTypeSymbol;
class AliasSymbol;
class ModuleSymbol;

struct FunctionTypeInfo;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* enclosingScope() const = 0;
    virtual Symbol* resolve(const std::string& name) const = 0;
    virtual Symbol* resolveHere(const std::string& name) const = 0;

    virtual std::string getScopeName() const = 0;
    virtual bool isUnsafeBlock() const;

    virtual void define(std::unique_ptr<TemplateSymbol> templ) = 0;
    virtual void define(std::unique_ptr<BuiltInTypeSymbol> builtin) = 0;
    virtual void define(std::unique_ptr<FunctionalSymbol> func) = 0;
    virtual void define(std::unique_ptr<VarSymbol> var) = 0;
    virtual void define(std::unique_ptr<AliasSymbol> type) = 0;
    virtual void define(std::unique_ptr<TypeSymbol> type) = 0;

    virtual void define(std::shared_ptr<ModuleSymbol> module) = 0;

    virtual TypeSymbol* resolveType    (const std::string& name) const;
    virtual FunctionalSymbol* resolveFunction(const std::string& name, const FunctionTypeInfo& info) const;
    virtual VarSymbol* resolveVariable(const std::string& name) const;

    virtual std::vector<VarSymbol*> getVars() const = 0;
    virtual std::vector<FunctionalSymbol*> getFunctions() const = 0;
    virtual std::vector<Symbol*> allSymbols() const = 0;
};

#endif
