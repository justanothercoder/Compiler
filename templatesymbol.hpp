#ifndef _TEMPLATESYMBOL_HPP_
#define _TEMPLATESYMBOL_HPP_

#include <vector>
#include <string>

class Symbol;
class ExprNode;

#include "templateparam.hpp"

class TemplateSymbol
{
    virtual Symbol* getSpec(std::vector<TemplateParam> symbols) const = 0;
};

#endif
