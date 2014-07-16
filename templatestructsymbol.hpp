#ifndef _TEMPLATESTRUCTSYMBOL_HPP_
#define _TEMPLATESTRUCTSYMBOL_HPP_

#include <algorithm>

#include "structsymbol.hpp"
#include "exprnode.hpp"
#include "templatesymbol.hpp"

#include "typeinfo.hpp"
#include "structsymbol.hpp"

class TemplateStructSymbol : public StructSymbol, public TemplateSymbol
{
public:

    TemplateStructSymbol(string name, Scope *enclosing_scope, const std::vector< std::pair<string, TypeInfo> >& template_symbols, AST *holder);

    SymbolType getSymbolType() const override;

    bool isIn(string name) const;
    
    Symbol* getSpec(vector<ExprNode*> symbols) const override;
    
public:

//	mutable map< vector<ExprNode*>, StructSymbol*> specs;   
	mutable map< long long, StructSymbol*> specs;   

	std::vector< std::pair<string, TypeInfo> > template_symbols;
    AST *holder;
};

#endif
