#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "structsymbol.hpp"
#include "ast.hpp"
#include "templatedeclholder.hpp"

class TemplateStructSymbol : public StructSymbol
{
public:

    TemplateStructSymbol(string name, Scope *enclosing_scope, const vector<Symbol*>& template_symbols);

    virtual SymbolType getSymbolType() const;

    const vector<Symbol*>& getTemplateSymbols() const;

    bool isIn(Symbol *sym) const;
    
public:

    TemplateDeclHolder* holder;
    
    vector<Symbol*> template_symbols;
};

#endif
