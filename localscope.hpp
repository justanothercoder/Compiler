#ifndef _LOCALSCOPE_HPP_
#define _LOCALSCOPE_HPP_

#include "basescope.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"

#include "globalscope.hpp"

class LocalScope : public BaseScope
{
public:

    LocalScope(Scope *enclosing_scope);

    virtual void accept(ScopeVisitor *visitor);

    virtual Scope* getEnclosingScope() const;
    virtual string getScopeName() const;

	virtual VarAllocator* get_valloc() const;
	
private:

    Scope *enclosing_scope;
    
};

#endif
