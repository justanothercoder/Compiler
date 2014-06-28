#include "functiontypeinfo.hpp"

FunctionTypeInfo::FunctionTypeInfo(std::vector<VariableType> params_types) : params_types(params_types) 
{

}
	
FunctionTypeInfo::FunctionTypeInfo(std::initializer_list<VariableType> init_list) : params_types(init_list)
{

}

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs) 
{ 
	return lhs.hash_func() < rhs.hash_func(); 
}

string FunctionTypeInfo::toString() const
{
	string res;

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
	
long long FunctionTypeInfo::hash_func() const
{
	long long res = 0;

	std::hash<std::string> hash_fn;

	for ( auto type : params_types )
		res += hash_fn(type.getName());

	return res;
};
