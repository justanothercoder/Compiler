#ifndef _OVERLOADEDFUNCTIONSYMBOL_HPP_
#define _OVERLOADEDFUNCTIONSYMBOL_HPP_

#include "symbol.hpp"
#include "templateparam.hpp"
#include "functiontraits.hpp"
#include "functionaltype.hpp"
#include "overloadedfunctiontypeinfo.hpp"

class TemplateSymbol;
class TemplateFunctionSymbol;

class OverloadedFunctionSymbol : public Symbol, public FunctionalType
{
    friend class PartiallyInstantiatedFunctionSymbol;

public:

    OverloadedFunctionSymbol(std::string name, FunctionTraits traits);
    std::string getName() const override;

    const FunctionSymbol* overloadWith(FunctionTypeInfo info) const;
    void addOverload(FunctionTypeInfo type_info, std::shared_ptr<const Symbol> sym) const;

    std::vector<const FunctionSymbol*> allOverloads() const;

    bool isMethod() const;
    bool isFunction() const override;

    const TemplateFunctionSymbol* templateFunction() const;
    void setTemplateFunction(const TemplateFunctionSymbol* function) const;
    
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;

private:

    VariableType getBaseType() const;

    const FunctionSymbol* getViableOverload(FunctionTypeInfo params_type) const;

private:

    std::string name;
    mutable OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
    mutable const TemplateFunctionSymbol* template_function = nullptr;
};

#endif
