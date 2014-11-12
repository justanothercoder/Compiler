#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"

TemplateInfo::TemplateInfo() : TemplateInfo(nullptr, { })
{

}

TemplateInfo::TemplateInfo(TemplateStructSymbol *sym, std::vector<TemplateParam> expr) : sym(sym), expr(expr)
{

}

boost::optional<TemplateParam> TemplateInfo::getReplacement(std::string name) const
{
    for ( size_t i = 0; i < sym -> template_symbols.size(); ++i )
    {
        if ( sym -> template_symbols[i].first == name )
            return expr[i];
    }
    return boost::none;
}
