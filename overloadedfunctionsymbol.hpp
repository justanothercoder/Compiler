#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "functionaltype.hpp"
#include "symbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "functiontraits.hpp"
#include "templateparam.hpp"
#include "functiondeclarationinfo.hpp"

class TemplateSymbol;
class TemplateFunctionSymbol;

class OverloadedFunctionSymbol : public Symbol, public FunctionalType
{
public:

    OverloadedFunctionSymbol(std::string name, OverloadedFunctionTypeInfo type_info, FunctionTraits traits);

    std::string getName() const override;

    OverloadedFunctionTypeInfo getTypeInfo() const;
    void addOverload(FunctionTypeInfo type_info, FunctionSymbol *sym) const;

    bool isMethod() const;
    SymbolType getSymbolType() const override;

    TemplateFunctionSymbol* templateFunction() const;
    void setTemplateFunction(TemplateFunctionSymbol* function) const;
    
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;

private:

    VariableType getBaseType() const;

    const FunctionSymbol* getViableOverload(FunctionTypeInfo params_type) const;

    bool checkValues(std::vector<ValueInfo> arguments, std::vector<VariableType> params) const;
    std::vector<ConversionInfo> getConversions(std::vector<ValueInfo> arguments, std::vector<VariableType> params) const;
    ConversionInfo getConversionInfo(const Type *lhs, const Type *rhs) const;

    const FunctionSymbol* overloadOfTemplateFunction(TemplateFunctionSymbol* template_function, FunctionTypeInfo info, std::vector<TemplateParam> partial = { }) const;
    boost::optional< std::map<std::string, TemplateParam> > makeMappingOfParams(TemplateSymbol* tmpl, std::vector<ParamInfo> formal_params, FunctionTypeInfo arguments) const;

private:

    std::string name;
    mutable OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
    mutable TemplateFunctionSymbol* template_function;
};

#endif
