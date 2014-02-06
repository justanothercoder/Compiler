#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "structsymbol.hpp"
#include "exprnode.hpp"
#include "templatedeclholder.hpp"

class TemplateStructSymbol : public StructSymbol
{
public:

    TemplateStructSymbol(string name, Scope *enclosing_scope, const vector<Symbol*>& template_symbols, TemplateDeclHolder *holder);

    virtual SymbolType getSymbolType() const;

    const vector<Symbol*>& getTemplateSymbols() const;

    bool isIn(Symbol *sym) const;
    ExprNode* getReplacement(Symbol *sym, const vector<ExprNode*>& expr) const;
public:

    vector<Symbol*> template_symbols;
    TemplateDeclHolder *holder;    
};

#endif
