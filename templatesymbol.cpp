#include "templatesymbol.hpp"

bool TemplateSymbol::isIn(const std::string& name) const
{
    const auto& template_params = templateParams();

    return std::find_if(std::begin(template_params), std::end(template_params), [&](auto&& p) 
    { 
        return name == p.first; 
    }) != std::end(template_params);
}

bool TemplateSymbol::canBeSpecializedWith(const TemplateArguments& arguments)
{
    const auto& template_params = templateParams();

    if ( template_params.size() != arguments.size() )
        return false;

    return specializeWith(arguments) != nullptr;
}
