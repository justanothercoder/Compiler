#include "functiontypeinfo.hpp"

FunctionTypeInfo::FunctionTypeInfo(VariableType return_type, vector<VariableType> params_types) : return_type(return_type), params_types(params_types) { }

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
{
    auto hash_func = [](const FunctionTypeInfo& fti)
	{
	    long long res = 0;

	    std::hash<std::string> hash_fn;
	    
	    res += hash_fn(fti.return_type.getName());
	    
	    for ( auto type : fti.params_types )
			res += hash_fn(type.getName());
	    
	    return res;
	};
    
    return hash_func(lhs) < hash_func(rhs);
}

string FunctionTypeInfo::toString() const
{
	string res = return_type.getName();

	res += "(";

	if ( !params_types.empty() )
	{
		auto it = std::begin(params_types);

		res += (*it).getName();
		for ( ++it; it != std::end(params_types); ++it )
			res += ", " + (*it).getName();
	}

	res += ")";

	return res;	
}
