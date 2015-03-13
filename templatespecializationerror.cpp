#include "templatespecializationerror.hpp"


TemplateSpecializationError::TemplateSpecializationError(const TemplateSymbol* sym, const TemplateArguments& args) : SemanticError(makeErrorMessage(sym, args)) { }

std::string TemplateSpecializationError::makeErrorMessage(const TemplateSymbol* sym, const TemplateArguments& args)
{
    std::string result = "";

    result += "Cannot specialize '" + sym -> getName() + "' with '";
    result += "<";

    if ( !args.empty() )
    {
        auto it = std::begin(args);
        result += toString(*it);
        ++it;

        while ( it != std::end(args) )
        {
            result += ", " + toString(*it);
            ++it;
        }
    }

    result += ">'";
    return result;
}
