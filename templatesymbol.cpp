#include "templatesymbol.hpp"

bool TemplateSymbol::isIn(std::string name) const
{
    const auto& template_symbols = templateSymbols();

    return std::find_if(std::begin(template_symbols),
                        std::end(template_symbols),
                        [&](auto&& p) { return name == p.first; }
    ) != std::end(template_symbols);
}
