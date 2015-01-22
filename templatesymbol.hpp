#ifndef _TEMPLATESYMBOL_HPP_
#define _TEMPLATESYMBOL_HPP_

#include <vector>
#include <string>
#include "templateparam.hpp"
#include "symbol.hpp"

class TemplateDeclarationNode;

class TemplateSymbol : public Symbol
{
public:
    virtual TemplateDeclarationNode* holder() const = 0;
    virtual TemplateParamsList templateSymbols() const = 0;

    bool isIn(std::string name) const;
};

#endif
