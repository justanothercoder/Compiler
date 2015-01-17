#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <string>
#include <set>
#include <memory>

class Type;
class Symbol;
class ScopeVisitor;
class FunctionalType;
struct TemplateInfo;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* enclosingScope() const = 0;
    virtual const Symbol* resolve(std::string name) const = 0;
    virtual const Symbol* resolveHere(std::string name) const = 0;

    virtual std::string getScopeName() const = 0;

    virtual void accept(ScopeVisitor& visitor) = 0;

    virtual const TemplateInfo& templateInfo() const = 0;
    virtual bool isUnsafeBlock() const;

    void define(std::shared_ptr<const Symbol> sym);

    const Type* resolveType(const std::string& name);
    const FunctionalType* resolveFunction(const std::string& name);
};

#endif
