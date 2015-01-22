#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"

TemplateInfo::TemplateInfo() : TemplateInfo(nullptr, { }) { }

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

bool TemplateInfo::isIn(std::string name) const { return sym -> isIn(name); }

std::string TemplateInfo::getInstName() const
{
    auto templates_name = std::string("");
    templates_name += sym -> getName() + "~";
    for ( auto param : template_params )
    {
        if ( param.which() == 0 )
            templates_name += boost::get<TypeInfo>(param).name();
        else
            templates_name += std::to_string(boost::get<int>(param));
    }

    return templates_name;
}
