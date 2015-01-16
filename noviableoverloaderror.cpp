#include "noviableoverloaderror.hpp"

NoViableOverloadError::NoViableOverloadError(std::string name, std::vector<VariableType> params_types)
    : SemanticError("No viable overload of '" + name + "' with types " + paramsToString(params_types) + ".")
{

}

NoViableOverloadError::NoViableOverloadError(std::string name, std::vector<ValueInfo> arguments)
    : SemanticError("No viable overload of '" + name + "' with types " + paramsToString(arguments) + ".")
{

}

std::string NoViableOverloadError::paramsToString(std::vector<VariableType> params_types)
{
    auto params_types_names = std::string("(");

    if ( !params_types.empty() )
    {
        auto it = std::begin(params_types);
        params_types_names += it -> getName();

        for ( ++it; it != std::end(params_types); ++it )
            params_types_names += ", " + it -> getName();
    }
    params_types_names += ")";

    return params_types_names;
}

std::string NoViableOverloadError::paramsToString(std::vector<ValueInfo> arguments)
{
    auto arguments_names = std::string("(");
    
    if ( !arguments.empty() )
    {
        auto it = std::begin(arguments);
        arguments_names += it -> type().getName();

        for ( ++it; it != std::end(arguments); ++it )
            arguments_names += ", " + it -> type().getName();
    }
    arguments_names += ")";

    return arguments_names;
}
