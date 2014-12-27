#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "templatesymbol.hpp"
#include "typeinfo.hpp"
#include "structsymbol.hpp"
#include "templateparam.hpp"

class DeclarationNode;
class StructSymbol;
class StructDeclarationNode;

class TemplateStructSymbol : public TemplateSymbol
{
public:

    TemplateStructSymbol(std::string name, TemplateParamsList template_symbols, TemplateDeclarationNode* _holder);

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
