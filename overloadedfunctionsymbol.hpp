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

    const OverloadedFunctionTypeInfo& getTypeInfo() const;
    void addOverload(FunctionTypeInfo type_info, std::shared_ptr<const Symbol> sym) const;

    bool isMethod() const;
    SymbolType getSymbolType() const override;

    const TemplateFunctionSymbol* templateFunction() const;
    void setTemplateFunction(const TemplateFunctionSymbol* function) const;
    
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;

private:

    VariableType getBaseType() const;

    const FunctionSymbol* getViableOverload(FunctionTypeInfo params_type) const;

    const FunctionSymbol* overloadOfTemplateFunction(const TemplateFunctionSymbol* template_function
                                                   , FunctionTypeInfo info
                                                   , const std::vector<TemplateParam>& partial = { }) const;

private:

    std::string name;
    mutable OverloadedFunctionTypeInfo type_info;

    FunctionTraits traits;
    mutable const TemplateFunctionSymbol* template_function = nullptr;
};

#endif
