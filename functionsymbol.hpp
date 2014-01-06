#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "basescope.hpp"

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
public:

    FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, bool is_operator = false);

    virtual void define(Symbol *sym);
    virtual Scope* getEnclosingScope();

    void setTypeInfo(FunctionTypeInfo function_type_info);

    string getTypedName();
    string getScopedTypedName();
    
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

    int params_size;
    
    string name;
    
    FunctionTypeInfo function_type_info;

    Scope *enclosing_scope;

    string scope_name;

    bool is_operator;

    Scope *symbol_scope;
};

#endif
