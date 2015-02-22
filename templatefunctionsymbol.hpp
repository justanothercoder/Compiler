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

    TemplateFunctionSymbol(const std::string& name, TemplateParamsInfo template_symbols, TemplateDeclarationNode* _holder);

    std::string getName() const override;

    TemplateDeclarationNode* holder() const override;
    TemplateParamsInfo templateSymbols() const override;

    FunctionSymbol* overloadOfTemplateFunction(FunctionTypeInfo info, const TemplateArguments& partial = { }) const;

private:

    std::string name;

    TemplateParamsInfo template_symbols;
    TemplateDeclarationNode* _holder;
};

#endif
