#ifndef _TEMPLATEFUNCTIONSYMBOL_HPP_
#define _TEMPLATEFUNCTIONSYMBOL_HPP_

#include <algorithm>

#include "templatesymbol.hpp"
#include "typeinfo.hpp"
#include "functionsymbol.hpp"
#include "templateargument.hpp"

class DeclarationNode;
class FunctionSymbol;

class TemplateFunctionSymbol : public TemplateSymbol
{
public:

    TemplateFunctionSymbol(std::string name, TemplateParamsInfo template_symbols, TemplateDeclarationNode* _holder);

    std::string getName() const override;

    TemplateDeclarationNode* holder() const override;
    TemplateParamsInfo templateParams() const override;

    FunctionSymbol* overloadOfTemplateFunction(FunctionTypeInfo info, const TemplateArguments& partial = { }) const;
    
    Symbol* specializeWith(const TemplateArguments& arguments) override;

    bool canBeSpecializedWith(const TemplateArguments& args, const FunctionTypeInfo& info);

private:

    std::string name;

    TemplateParamsInfo template_params;
    TemplateDeclarationNode* _holder;
};

#endif
