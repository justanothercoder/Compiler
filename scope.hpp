#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <string>

class Scope;
class Symbol;
class ScopeVisitor;
class VarAllocator;
class TempAllocator;
class VariableSymbol;
struct TemplateInfo;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* getEnclosingScope() const = 0;
    virtual Symbol* resolve(std::string name) const = 0;

	virtual VarAllocator& getVarAlloc() = 0;
	virtual TempAllocator& getTempAlloc() = 0;

    virtual std::string getScopeName() const = 0;

    virtual void accept(ScopeVisitor *visitor) = 0;

	virtual const TemplateInfo& getTemplateInfo() const = 0;
	virtual bool isUnsafeBlock() const = 0;
};

#endif
