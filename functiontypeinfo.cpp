#include "functiontypeinfo.hpp"

FunctionTypeInfo::FunctionTypeInfo(Type *return_type, const vector<Type*>& params_types, bool is_method) : return_type(return_type), params_types(params_types), is_method(is_method)
{

}

Type* FunctionTypeInfo::getReturnType() const
{
    return return_type;    
}

Type* FunctionTypeInfo::getParamType(int i) const
{
    return params_types[i];
}

int FunctionTypeInfo::getNumberOfParams() const
{
    return params_types.size();
}

bool FunctionTypeInfo::isMethod() const
{
    return is_method;
}

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
{
    auto hash_func = [](const FunctionTypeInfo& fti)
	{
	    long long res = 0;

	    res += std::hash<std::string>()(fti.getReturnType()->getName());

	    for ( int i = 0; i < fti.getNumberOfParams(); ++i )
		res += std::hash<std::string>()(fti.getParamType(i)->getName());

	    return res;
	};
    
    return hash_func(lhs) < hash_func(rhs);
}
