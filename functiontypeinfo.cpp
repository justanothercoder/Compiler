#include "functiontypeinfo.hpp"

FunctionTypeInfo::FunctionTypeInfo(Type *return_type, const vector<Type*>& params_types) : return_type(return_type), params_types(params_types)
{

}

Type* FunctionTypeInfo::getReturnType()
{
    return return_type;    
}

Type* FunctionTypeInfo::getParamType(int i)
{
    return params_types[i];
}

int FunctionTypeInfo::getNumberOfParams()
{
    return params_types.size();
}

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
{
    return ((&lhs) < (&rhs));
}
