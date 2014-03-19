#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "symbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "globalconfig.hpp"
#include "functiontraits.hpp"

class OverloadedFunctionSymbol : public Symbol, public Type
{
public:

    OverloadedFunctionSymbol(string name, OverloadedFunctionTypeInfo type_info, FunctionTraits traits);

    virtual string getName() const;
    virtual int getSize() const;

    OverloadedFunctionTypeInfo getTypeInfo() const;

    void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym);

    bool isMethod() const;
    bool isConstructor() const;
    bool isOperator() const;

    VariableType getBaseType() const;

    virtual SymbolType getSymbolType() const;
    virtual TypeKind getTypeKind() const;
    
private:

    string name;
    OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
};

#endif
