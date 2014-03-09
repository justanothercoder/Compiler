#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "alldeps.hpp"

#include "basescope.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

#include "varallocator.hpp"

using std::map;

class GlobalScope : public BaseScope
{
public:

    GlobalScope();

    virtual void accept(ScopeVisitor *visitor);

    virtual Scope* getEnclosingScope() const;
    virtual string getScopeName() const;
	
	virtual VarAllocator* get_valloc() const;

private:

	mutable VarAllocator valloc;
};

#endif
