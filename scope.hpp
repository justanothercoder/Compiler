#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <string>
#include <set>
#include <memory>
#include "typeinfo.hpp"
#include "templateparam.hpp"

class Symbol;
class ScopeVisitor;
struct TemplateInfo;
class Type;
class FunctionalType;

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
