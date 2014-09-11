#include "noviableoverloaderror.hpp"
	
NoViableOverloadError::NoViableOverloadError(std::string name, std::vector<const Type*> params_types) 
	: SemanticError("No viable overload of '" + name + "' with types " + paramsToString(params_types) + ".")
{

}

std::string NoViableOverloadError::paramsToString(std::vector<const Type*> params_types)
{
	std::string params_types_names = "(";
	if ( !params_types.empty() )
	{
		auto it = std::begin(params_types);			
		params_types_names += (*it) -> getName();

		for ( ++it; it != std::end(params_types); ++it )
			params_types_names += ", " + (*it) -> getName();
	}
	params_types_names += ")";
	
	return params_types_names;
}

