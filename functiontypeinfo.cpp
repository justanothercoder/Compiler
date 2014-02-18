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
	    
		auto& pt = fti.getParamsTypes();

	    for ( auto type : pt )
			res += hash_fn(type->getName());
	    
	    return res;
	};
    
    return hash_func(lhs) < hash_func(rhs);
}
