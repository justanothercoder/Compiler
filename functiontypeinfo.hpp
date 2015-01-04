#ifndef _FUNCTIONTYPEINFO_HPP_
#define _FUNCTIONTYPEINFO_HPP_

#include <initializer_list>
#include <vector>
#include <string>
#include "variabletype.hpp"

struct FunctionTypeInfo
{
public:

    FunctionTypeInfo(std::vector<VariableType> params_types);
    FunctionTypeInfo(std::initializer_list<VariableType> init_list);

    std::string toString() const;

    long long hash_func() const;

    bool isCompatibleWith(const FunctionTypeInfo& info) const;

    int rankOfConversion(const FunctionTypeInfo& info) const;

    const std::vector<VariableType>& params() const;
    VariableType paramAt(int k) const;

private:

    std::vector<VariableType> params_types;
};

bool operator==(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);
bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);

#endif
