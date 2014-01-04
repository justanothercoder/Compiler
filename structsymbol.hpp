#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_

#include <map>

#include "symbol.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "variablesymbol.hpp"

using std::map;

class StructSymbol : public Symbol, public Scope, public Type
{
public:
    
    StructSymbol(string name, Scope *enclosing_scope);

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

    virtual string getName();
    virtual int getSize();

    virtual Type* getTypeHint(ExprNode *expr);
    virtual void setTypeHint(ExprNode *expr, Type *type); 

    virtual int getAddress(VariableSymbol *sym);

    virtual int getScopeAddress();
    virtual int getScopeSize();

    virtual string getScopeName();

    virtual Scope* getScope();
    virtual void setScope(Scope *scope);    
    
private:

    map<ExprNode*, Type*> type_hints;
    map<string, Symbol*> members;

    string name;
    
    Scope *enclosing_scope;
    
    int type_size;

    string scope_name;

    Scope *symbol_scope;
};
		     
#endif
