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

    string getName() const override;
    int getSize() const override;

    OverloadedFunctionTypeInfo getTypeInfo() const;

    void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym);

    bool isMethod() const;
    bool isConstructor() const;
    bool isOperator() const;

    VariableType getBaseType() const;

    SymbolType getSymbolType() const override;
    TypeKind getTypeKind() const override;
    
	FunctionSymbol* getViableOverload(FunctionTypeInfo params_type);
    
private:

    string name;
    OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
};

#endif
