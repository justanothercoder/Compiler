#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "symbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"

class OverloadedFunctionSymbol : public Symbol, public Type
{
public:

    OverloadedFunctionSymbol(string name, OverloadedFunctionTypeInfo type_info);

    virtual string getName();
    virtual int getSize();

    virtual Scope* getScope();
    virtual void setScope(Scope *scope);

    OverloadedFunctionTypeInfo getTypeInfo();

    void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym);
    
private:

    string name;
    OverloadedFunctionTypeInfo type_info;

    Scope *symbol_scope;
};

#endif
