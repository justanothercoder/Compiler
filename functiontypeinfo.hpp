#ifndef _FUNCTIONTYPEINFO_HPP_
#define _FUNCTIONTYPEINFO_HPP_

#include <vector>

#include "type.hpp"

using std::vector;

class FunctionTypeInfo
{
public:

    FunctionTypeInfo(Type *return_type, const vector<Type*>& params_types);

    Type* getReturnType();
    Type* getParamType(int i);

    int getNumberOfParams();
    
private:

    Type *return_type;
    vector<Type*> params_types;
};

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);

#endif
