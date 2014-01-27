#include "templatestructsymbol.hpp"

TemplateStructSymbol::TemplateStructSymbol(string name, Scope *enclosing_scope, const vector<Symbol*>& template_symbols) : StructSymbol(name, enclosing_scope), template_symbols(template_symbols)
{
    
}
