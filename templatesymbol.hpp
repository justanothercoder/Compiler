#ifndef _TEMPLATESYMBOL_HPP_
#define _TEMPLATESYMBOL_HPP_

#include <vector>
#include <string>

class Scope;
class Symbol;
class ExprNode;

#include "templateparam.hpp"

class TemplateSymbol
{
    virtual Symbol* getSpec(std::vector<TemplateParam> symbols, Scope *inst_scope) const = 0;
};

#endif
