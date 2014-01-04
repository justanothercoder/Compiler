#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "scope.hpp"
#include "overloadedfunctionsymbol.hpp"

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public Scope, public Type
{
public:

    FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, bool is_operator = false);

    virtual Scope* getEnclosingScope();
    virtual Symbol* resolve(string name);
    virtual void define(Symbol *sym);

    void setTypeInfo(FunctionTypeInfo function_type_info);

    virtual Type* getTypeHint(ExprNode *expr);
    virtual void setTypeHint(ExprNode *expr, Type *type); 

    string getTypedName();

    virtual int getAddress(VariableSymbol *sym);
 
    virtual int getScopeAddress();
    virtual int getScopeSize();

    virtual string getScopeName();

    void recalc_scope_address();

    bool isOperator();

    virtual Scope* getScope();
    virtual void setScope(Scope *scope);    

    virtual string getName();
    virtual int getSize();
    
    FunctionTypeInfo getTypeInfo();
    
private:

    int scope_address, scope_size;
    
    string name;
    
    FunctionTypeInfo function_type_info;

    Scope *enclosing_scope;

    map<ExprNode*, Type*> type_hints;
    map<string, Symbol*> members;
    map<VariableSymbol*, int> addresses;

    string scope_name;

    bool is_operator;

    Scope *symbol_scope;
};

#endif
