#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <string>
#include <set>

#include "typeinfo.hpp"
#include "templateparam.hpp"

class Type;
class Scope;
class Symbol;
class ScopeVisitor;
class VarAllocator;
class TempAllocator;
class VariableSymbol;
class FunctionSymbol;
struct TemplateInfo;

class TemplateStructSymbol;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* enclosingScope() const = 0;
    virtual Symbol* resolve(std::string name) const = 0;
    virtual Symbol* resolveHere(std::string name) const = 0;

    virtual VarAllocator& varAlloc() const = 0;
    virtual TempAllocator& tempAlloc() const = 0;

    virtual std::string getScopeName() const = 0;

    virtual void accept(ScopeVisitor& visitor) = 0;

    virtual const TemplateInfo& templateInfo() const = 0;
    virtual bool isUnsafeBlock() const;

    void define(Symbol *sym);

    const Type* resolveType(std::string name);
};

#endif
