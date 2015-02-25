#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "templatesymbol.hpp"
#include "typeinfo.hpp"
#include "structsymbol.hpp"
#include "templateargument.hpp"

class DeclarationNode;
class StructSymbol;
class StructDeclarationNode;

class TemplateStructSymbol : public TemplateSymbol
{
public:

    TemplateStructSymbol(std::string name, TemplateParamsInfo template_params, TemplateDeclarationNode* _holder);

    std::string getName() const override;

    bool isIn(std::string name) const;
    
    TemplateDeclarationNode* holder() const override;
    TemplateParamsInfo templateParams() const override;
    
    Symbol* specializeWith(const TemplateArguments& arguments) override;
    
private:

    std::string name;

    TemplateParamsInfo template_params;
    TemplateDeclarationNode* _holder;
};

#endif
