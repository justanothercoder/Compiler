#ifndef _TEMPLATESYMBOL_HPP_
#define _TEMPLATESYMBOL_HPP_

#include <vector>
#include <string>

class Symbol;
class ExprNode;

class TemplateSymbol
{
    virtual Symbol* getSpec(std::vector<ExprNode*> symbols) const = 0;
};

#endif
