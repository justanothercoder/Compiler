#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include "symbol.hpp"
#include "functiontype.hpp"

class ExprNode;
class VariableSymbol;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* getEnclosingScope() = 0;
    virtual Symbol* resolve(string name) = 0;
    virtual void define(Symbol *sym) = 0;

    virtual Type* getTypeHint(ExprNode *expr) = 0;
    virtual void setTypeHint(ExprNode *expr, Type *type) = 0;

    virtual int getAddress(VariableSymbol* sym) = 0;

    virtual int getScopeAddress() = 0;
    virtual int getScopeSize() = 0;

    virtual string getScopeName() = 0;
};

#endif
