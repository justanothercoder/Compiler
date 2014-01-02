#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "typedsymbol.hpp"
#include "overloadedfunctiontype.hpp"
#include "variablesymbol.hpp"
#include "scope.hpp"

using std::map;
using std::pair;

class FunctionSymbol : public TypedSymbol, public Scope
{
public:

    FunctionSymbol(string name, FunctionType *function_type, Scope *enclosing_scope, bool is_operator = false);

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

    virtual Type* getType();

    void setType(FunctionType *function_type);

    virtual Type* getTypeHint(ExprNode *expr);
    virtual void setTypeHint(ExprNode *expr, Type *type); 

    string getTypedName();

    virtual int getAddress(VariableSymbol *sym);
 
    virtual int getScopeAddress();
    virtual int getScopeSize();

    virtual string getScopeName();

    void recalc_scope_address();

    bool isOperator();
    
private:

    int scope_address, scope_size;
    
    FunctionType *function_type;

    Scope *enclosing_scope;

    map<ExprNode*, Type*> type_hints;
    map<string, Symbol*> members;
    map<VariableSymbol*, int> addresses;

    string scope_name;

    bool is_operator;
};

#endif
