#include "templatestructsymbol.hpp"

TemplateStructSymbol::TemplateStructSymbol(string name, Scope *enclosing_scope, const vector<Symbol*>& template_symbols) : StructSymbol(name, enclosing_scope), template_symbols(template_symbols)
{
    
}

StructSymbol* TemplateStructSymbol::getSpec(const vector<ExprNode*>& symbols) const
{
    auto it = specs.find(symbols);
    if ( it != std::end(specs) )
	return it->second;

    return nullptr;
}

SymbolType TemplateStructSymbol::getSymbolType() const { return SymbolType::TEMPLATESTRUCT; }

const vector<Symbol*>& TemplateStructSymbol::getTemplateSymbols() const { return template_symbols; }
