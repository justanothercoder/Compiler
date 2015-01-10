#ifndef _PARTIALLYINSTANTIATEDFUNCTIONSYMBOL_HPP_
#define _PARTIALLYINSTANTIATEDFUNCTIONSYMBOL_HPP_

#include "overloadedfunctionsymbol.hpp"

class PartiallyInstantiatedFunctionSymbol : public FunctionalType
{
public:

    PartiallyInstantiatedFunctionSymbol(OverloadedFunctionSymbol* ov_func, std::vector<TemplateParam> tmpl_arguments);

    std::string getName() const override;
    CallInfo resolveCall(std::vector<ValueInfo> arguments) const override;

private:
    OverloadedFunctionSymbol* ov_func;
    std::vector<TemplateParam> tmpl_arguments;
};

#endif
