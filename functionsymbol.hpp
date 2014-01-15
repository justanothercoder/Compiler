#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "basescope.hpp"
#include "globalconfig.hpp"

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
public:

    FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, bool is_operator = false, bool is_method = false);

    virtual void define(Symbol *sym);
    virtual Scope* getEnclosingScope() const;

    void setTypeInfo(FunctionTypeInfo function_type_info);

    string getTypedName() const;
    string getScopedTypedName() const;
    
    virtual int getScopeSize() const;
    virtual string getScopeName() const;

    void recalc_scope_address();

    bool isOperator() const;
    bool isMethod() const;

    virtual string getName() const;
    virtual int getSize() const;
    
    FunctionTypeInfo getTypeInfo() const;
    
private:

    int params_size;
    
    string name;
    
    FunctionTypeInfo function_type_info;

    Scope *enclosing_scope;

    string scope_name;

    bool is_operator, is_method;    
};

#endif
