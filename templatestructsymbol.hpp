#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include "structsymbol.hpp"

class TemplateStructSymbol : public StructSymbol
{
public:

    TemplateStructSymbol(string name, Scope *enclosing_scope, const vector<Symbol*>& template_symbols);

    StructSymbol* getSpec(const vector<ExprNode*>& symbols) const;

public:
    vector<Symbol*> template_symbols;
    mutable map< vector<ExprNode*>, StructSymbol*> specs;
};

#endif
