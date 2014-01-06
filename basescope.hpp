#ifndef _BASESCOPE_HPP_
#define _BASESCOPE_HPP_

#include <map>

#include "scope.hpp"

using std::map;

class ExprNode;

class BaseScope : public Scope
{
public:

    BaseScope();

    virtual Symbol* resolve(string name);

    virtual Type* getTypeHint(ExprNode *expr);
    virtual void setTypeHint(ExprNode *expr, Type *type);

    virtual int getAddress(VariableSymbol *sym);

    virtual int getScopeSize();
    virtual int getScopeAddress();

protected:

    map<ExprNode*, Type*> type_hints;
    map<string, Symbol*> table;
    map<VariableSymbol*, int> addresses;

    int scope_size, scope_address;

};

#endif
