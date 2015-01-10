#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"

TemplateInfo::TemplateInfo() : TemplateInfo(nullptr, { })
{

}

TemplateInfo::TemplateInfo(const TemplateSymbol *sym, std::vector<TemplateParam> template_params) : sym(sym), template_params(template_params)
{

}

boost::optional<TemplateParam> TemplateInfo::getReplacement(std::string name) const
{
    auto template_symbols = sym -> templateSymbols();
    for ( size_t i = 0; i < template_symbols.size(); ++i )
    {
        if ( template_symbols[i].first == name )
            return template_params[i];
    }
    return boost::none;
}

bool TemplateInfo::isIn(std::string name) const
{
    auto template_symbols = sym -> templateSymbols();

    return std::find_if(std::begin(template_symbols),
                        std::end(template_symbols),
                        [&](auto p) { return name == p.first; }
    ) != std::end(template_symbols);
}
