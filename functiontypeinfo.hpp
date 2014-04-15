#ifndef _FUNCTIONTYPEINFO_HPP_
#define _FUNCTIONTYPEINFO_HPP_

#include <vector>

#include "variabletype.hpp"

using std::vector;

struct FunctionTypeInfo
{
public:

    FunctionTypeInfo(VariableType return_type, vector<VariableType> params_types);

	string toString() const;

	long long hash_func() const;

    VariableType return_type;
    vector<VariableType> params_types;
};

bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);

#endif
