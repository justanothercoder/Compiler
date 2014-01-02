#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "scope.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctiontype.hpp"
#include "variablesymbol.hpp"

using std::map;

class GlobalScope : public Scope
{
public:

    GlobalScope();

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

    virtual Type* getTypeHint(ExprNode *expr);
    virtual void setTypeHint(ExprNode *expr, Type *type);

    virtual int getAddress(VariableSymbol *sym);

    virtual int getScopeAddress();
    virtual int getScopeSize();

    virtual string getScopeName();
    
private:

    map<ExprNode*, Type*> type_hints;
    map<string, Symbol*> table;
    map<VariableSymbol*, int> addresses;

    int scope_size;
};

#endif
