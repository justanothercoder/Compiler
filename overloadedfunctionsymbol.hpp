#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "functionaltype.hpp"
#include "symbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "functiontraits.hpp"

class TemplateFunctionSymbol;

class OverloadedFunctionSymbol : public Symbol, public FunctionalType
{
public:

    OverloadedFunctionSymbol(std::string name, OverloadedFunctionTypeInfo type_info, FunctionTraits traits);

    std::string getName() const override;

    OverloadedFunctionTypeInfo getTypeInfo() const;

    void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym) const;

    bool isMethod() const;
    bool isConstructor() const;
    bool isOperator() const;

    VariableType getBaseType() const;

    SymbolType getSymbolType() const override;

    const FunctionSymbol* getViableOverload(FunctionTypeInfo params_type) const;

    TemplateFunctionSymbol* templateFunction() const;
    void setTemplateFunction(TemplateFunctionSymbol* function) const;
    
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;

private:

    bool checkValues(std::vector<ValueInfo> arguments, std::vector<VariableType> params) const;
    std::vector<ConversionInfo> getConversions(std::vector<ValueInfo> arguments, std::vector<VariableType> params) const;
    ConversionInfo getConversionInfo(const Type *lhs, const Type *rhs) const;

private:

    std::string name;
    mutable OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
    mutable TemplateFunctionSymbol* template_function;
};

#endif
