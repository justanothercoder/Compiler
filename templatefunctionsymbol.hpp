#ifndef _TEMPLATEFUNCTIONSYMBOL_HPP_
#define _TEMPLATEFUNCTIONSYMBOL_HPP_

#include <algorithm>

#include "templatesymbol.hpp"
#include "typeinfo.hpp"
#include "functionsymbol.hpp"
#include "templateparam.hpp"

class DeclarationNode;
class FunctionSymbol;

class TemplateFunctionSymbol : public TemplateSymbol
{
public:

    TemplateFunctionSymbol(const std::string& name, TemplateParamsList template_symbols, TemplateDeclarationNode* _holder);

    std::string getName() const override;

    TemplateDeclarationNode* holder() const override;
    TemplateParamsList templateSymbols() const override;

    std::unique_ptr<DefineSymbolVisitor> defineSymbolVisitor() const override;

    const FunctionSymbol* overloadOfTemplateFunction(FunctionTypeInfo info, const std::vector<TemplateParam>& partial = { }) const;

private:

    std::string name;

    TemplateParamsList template_symbols;
    TemplateDeclarationNode* _holder;
};

#endif
