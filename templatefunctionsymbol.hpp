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

    TemplateFunctionSymbol(std::string name, TemplateParamsList template_symbols, TemplateDeclarationNode* _holder);

    std::string getName() const override;
    SymbolType getSymbolType() const override;

    bool isIn(std::string name) const;
    
    TemplateDeclarationNode* holder() const override;
    TemplateParamsList templateSymbols() const override;

private:

    std::string name;

    TemplateParamsList template_symbols;
    TemplateDeclarationNode* _holder;
};

#endif
