#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"

TemplateInfo::TemplateInfo() : TemplateInfo(nullptr, { }) { }

TemplateInfo::TemplateInfo(const TemplateSymbol *sym, TemplateArguments template_arguments) : sym(sym), template_arguments(template_arguments)
{

}

boost::optional<TemplateArgument> TemplateInfo::getReplacement(std::string name) const
{
    auto template_symbols = sym -> templateSymbols();
    for ( size_t i = 0; i < template_symbols.size(); ++i )
    {
        if ( template_symbols[i].first == name )
            return template_arguments[i];
    }
    return boost::none;
}

bool TemplateInfo::isIn(std::string name) const { return sym -> isIn(name); }

std::string TemplateInfo::getInstName() const
{
    auto templates_name = std::string("");
    templates_name += sym -> getName() + "~";
    for ( auto param : template_arguments )
    {
        if ( param.which() == 0 ) {
            templates_name += boost::get<TypeInfo>(param).name();
        }
        else {
            templates_name += std::to_string(boost::get<int>(param));
        }
    }

    return templates_name;
}
