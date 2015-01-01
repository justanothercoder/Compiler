#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "type.hpp"
#include "symbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "functiontraits.hpp"

class TemplateFunctionSymbol;

class OverloadedFunctionSymbol : public Symbol, public Type
{
public:

    OverloadedFunctionSymbol(std::string name, OverloadedFunctionTypeInfo type_info, FunctionTraits traits);

    std::string getName() const override;
    size_t sizeOf() const override;

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

    FunctionSymbol* getConversionTo(const Type *type) const override;

private:

    std::string name;
    mutable OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
    TemplateFunctionSymbol* template_function;
};

#endif
