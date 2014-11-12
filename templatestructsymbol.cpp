#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "templateinfo.hpp"

#include "variablenode.hpp"

TemplateStructSymbol::TemplateStructSymbol(std::string name
        , Scope *enclosing_scope
        , std::vector< std::pair<std::string, TypeInfo> > template_symbols
        , AST *holder) : StructSymbol(name, enclosing_scope, (*new TemplateInfo()))
    , template_symbols(template_symbols)
    , holder(holder)
{

}

SymbolType TemplateStructSymbol::getSymbolType() const
{
    return SymbolType::TEMPLATESTRUCT;
}

bool TemplateStructSymbol::isIn(std::string name) const
{
    return std::find_if(std::begin(template_symbols),
                        std::end(template_symbols),
                        [&](std::pair<std::string, TypeInfo> p) { return name == p.first; }
    ) != std::end(template_symbols);
}
