#include "functiontype.hpp"

FunctionType::FunctionType(Type *return_type, const vector<Type*>& params_types) : return_type(return_type), params_types(params_types)
{
    type_size = sizeof(int*);
}

string FunctionType::getName()
{
    string res = return_type->getName();

    res += "(";

    if ( !params_types.empty() )
    {
	res += params_types.front()->getName();
	for ( auto i = std::begin(params_types) + 1; i != std::end(params_types); ++i )
	    res += ", " + (*i)->getName();
    }
    
    res += ")";

    return res;
}

int FunctionType::getSize()
{
    return type_size;
}

Type* FunctionType::getReturnType()
{
    return return_type;    
}

Type* FunctionType::getParamType(int i)
{
    return params_types[i];
}

int FunctionType::getNumberOfParams()
{
    return params_types.size();
}

