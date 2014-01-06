#ifndef _LOCALSCOPE_HPP_
#define _LOCALSCOPE_HPP_

#include "basescope.hpp"
#include "functionsymbol.hpp"

class LocalScope : public BaseScope
{
public:

    LocalScope(Scope *enclosing_scope);

    virtual void define(Symbol *sym);
    virtual Scope* getEnclosingScope();
    virtual string getScopeName();
    
    void recalc_scope_address();

private:

    Scope *enclosing_scope;
    
};

#endif
