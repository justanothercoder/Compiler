#ifndef _PARTIALLYINSTANTIATEDFUNCTIONSYMBOL_HPP_
#define _PARTIALLYINSTANTIATEDFUNCTIONSYMBOL_HPP_

#include "functionaltype.hpp"
#include "templateparam.hpp"

class OverloadedFunctionSymbol;

class PartiallyInstantiatedFunctionSymbol : public FunctionalType
{
public:

    PartiallyInstantiatedFunctionSymbol(const OverloadedFunctionSymbol* ov_func, std::vector<TemplateParam> tmpl_arguments);

    std::string getName() const override;
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;

private:
    const OverloadedFunctionSymbol* ov_func;
    std::vector<TemplateParam> tmpl_arguments;
};

#endif
