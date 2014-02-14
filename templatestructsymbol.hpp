#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "structsymbol.hpp"
#include "exprnode.hpp"
#include "templatesymbol.hpp"

#include "typeinfo.hpp"

class TemplateStructSymbol : public StructSymbol, public TemplateSymbol
{
public:

    TemplateStructSymbol(string name, Scope *enclosing_scope, const vector< pair<string, TypeInfo> >& template_symbols, AST *holder);

    virtual SymbolType getSymbolType() const;

    bool isIn(string name) const;
    
    ExprNode* getReplacement(string name, const vector<ExprNode*>& expr) const;

    virtual Symbol* getSpec(const vector<ExprNode*>& symbols) const;
    
public:

    static map< vector<ExprNode*>, StructSymbol*> specs;   

    vector< pair<string, TypeInfo> > template_symbols;
    AST *holder;
};

#endif
