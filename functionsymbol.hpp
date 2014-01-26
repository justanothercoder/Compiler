#ifndef _FUNCTIONSYMBOL_HPP_
#define _FUNCTIONSYMBOL_HPP_

#include <map>

#include "variablesymbol.hpp"
#include "basescope.hpp"
#include "globalconfig.hpp"
#include "functiontraits.hpp"
#include "functiontypeinfo.hpp"

#include "overloadedfunctionsymbol.hpp"

using std::map;
using std::pair;

class FunctionSymbol : public Symbol, public BaseScope, public Type
{
public:

    FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, FunctionTraits traits);

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
    bool isConstructor() const;

    virtual string getName() const;
    virtual int getSize() const;
    
    FunctionTypeInfo getTypeInfo() const;
    FunctionTraits getTraits() const;
    
    virtual SymbolType getSymbolType() const;
    virtual TypeKind getTypeKind() const;
    
private:

    int params_size;
    
    string name;
    
    FunctionTypeInfo function_type_info;

    Scope *enclosing_scope;

    string scope_name;

    FunctionTraits traits;
};

#endif
