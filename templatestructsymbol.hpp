#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "structsymbol.hpp"
#include "exprnode.hpp"
#include "templatedeclholder.hpp"
#include "templatesymbol.hpp"

class TemplateStructSymbol : public StructSymbol, public TemplateSymbol
{
public:

    TemplateStructSymbol(string name, Scope *enclosing_scope, const vector<Symbol*>& template_symbols, TemplateDeclHolder *holder);

    virtual SymbolType getSymbolType() const;

    bool isIn(Symbol *sym) const;
    bool isIn(string name) const;
    
    ExprNode* getReplacement(Symbol *sym, const vector<ExprNode*>& expr) const;
    ExprNode* getReplacement(string name, const vector<ExprNode*>& expr) const;

    virtual Symbol* getSpec(const vector<ExprNode*>& symbols) const;
    
public:

    static map< vector<ExprNode*>, StructSymbol*> specs;   

    vector<Symbol*> template_symbols;
    TemplateDeclHolder *holder;
    vector<string> template_classes;
};

#endif
