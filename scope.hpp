#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include "symbol.hpp"
#include "type.hpp"
#include "exprnode.hpp"

#include "scopevisitor.hpp"
#include "varallocator.hpp"
#include "tempallocator.hpp"

class VariableSymbol;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* getEnclosingScope() const = 0;
    virtual Symbol* resolve(string name) const = 0;

	virtual VarAllocator* get_valloc() = 0;
	virtual TempAllocator& getTempAlloc() = 0;

    virtual string getScopeName() const = 0;

    virtual void accept(ScopeVisitor *visitor) = 0;
};

#endif
