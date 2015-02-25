#ifndef _TEMPLATESYMBOL_HPP_
#define _TEMPLATESYMBOL_HPP_

#include <vector>
#include <string>
#include "templateargument.hpp"
#include "symbol.hpp"

class TemplateDeclarationNode;

class TemplateSymbol : public Symbol
{
public:
    virtual TemplateDeclarationNode* holder() const = 0;
    virtual TemplateParamsInfo templateParams() const = 0;

    virtual Symbol* specializeWith(const TemplateArguments& arguments) = 0;

    bool isIn(const std::string& name) const;
    bool canBeSpecializedWith(const TemplateArguments& arguments);
};

#endif
