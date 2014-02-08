#ifndef _LOCALSCOPE_HPP_
#define _LOCALSCOPE_HPP_

#include "basescope.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"

class LocalScope : public BaseScope
{
public:

    LocalScope(Scope *enclosing_scope);

    virtual void accept(ScopeVisitor *visitor);

    virtual void define(Symbol *sym);
    virtual Scope* getEnclosingScope() const;
    virtual string getScopeName() const;
    
    void recalc_scope_address();

private:

    Scope *enclosing_scope;
    
};

#endif
