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
    size_t getSize() const override;

    OverloadedFunctionTypeInfo getTypeInfo() const;

	void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym) const;

    bool isMethod() const;
    bool isConstructor() const;
    bool isOperator() const;

    VariableType getBaseType() const;

    SymbolType getSymbolType() const override;
    TypeKind getTypeKind() const override;
    
	FunctionSymbol* getViableOverload(FunctionTypeInfo params_type) const;

	bool isConvertableTo(const Type *type) const override;
	boost::optional<int> rankOfConversion(const Type *type) const override;
    
private:

    string name;
    OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
};

#endif
