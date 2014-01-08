#ifndef _FUNCTIONTYPEINFO_HPP_
#define _FUNCTIONTYPEINFO_HPP_

#include <vector>

#include "type.hpp"

using std::vector;

class FunctionTypeInfo
{
public:

    FunctionTypeInfo(Type *return_type, const vector<Type*>& params_types, bool is_method = false);

    Type* getReturnType() const;
    Type* getParamType(int i) const;

    int getNumberOfParams() const;

    bool isMethod() const;
    
private:

    Type *return_type;
    vector<Type*> params_types;
    bool is_method;
};

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);

#endif
