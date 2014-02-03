#include "templatestructsymbol.hpp"

TemplateStructSymbol::TemplateStructSymbol(string name, Scope *enclosing_scope, const vector<Symbol*>& template_symbols) : StructSymbol(name, enclosing_scope), template_symbols(template_symbols)
{
    
}

SymbolType TemplateStructSymbol::getSymbolType() const { return SymbolType::TEMPLATESTRUCT; }

const vector<Symbol*>& TemplateStructSymbol::getTemplateSymbols() const { return template_symbols; }

bool TemplateStructSymbol::isIn(Symbol *sym) const
{
    return std::accumulate(std::begin(template_symbols), std::end(template_symbols), false, [&](bool a, Symbol *s) { return a || (s == sym); });
}
