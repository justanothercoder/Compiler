#include "functiontypeinfo.hpp"

FunctionTypeInfo::FunctionTypeInfo(Type *return_type, const vector<Type*>& params_types) : return_type(return_type), params_types(params_types) { }

Type* FunctionTypeInfo::getReturnType() const { return return_type; }
Type* FunctionTypeInfo::getParamType(int i) const { return params_types[i]; }

int FunctionTypeInfo::getNumberOfParams() const { return params_types.size(); }

const vector<Type*>& FunctionTypeInfo::getParamsTypes() const { return params_types; }

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
{
    auto hash_func = [](const FunctionTypeInfo& fti)
	{
	    long long res = 0;

	    std::hash<std::string> hash_fn;
	    
	    res += hash_fn(fti.getReturnType()->getName());
	    
	    for ( auto type : fti.getParamsTypes() )
			res += hash_fn(type->getName());
	    
	    return res;
	};
    
    return hash_func(lhs) < hash_func(rhs);
}

string FunctionTypeInfo::toString() const
{
	string res = return_type->getName();

	res += "(";

	if ( !params_types.empty() )
	{
		auto it = std::begin(params_types);

		res += (*it)->getName();
		for ( ++it; it != std::end(params_types); ++it )
			res += ", " + (*it)->getName();
	}

	res += ")";

	return res;	
}
