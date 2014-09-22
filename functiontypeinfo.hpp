#ifndef _FUNCTIONTYPEINFO_HPP_
#define _FUNCTIONTYPEINFO_HPP_

#include <initializer_list>
#include <vector>
#include <string>

class Type;

struct FunctionTypeInfo
{
public:

    FunctionTypeInfo(std::vector<const Type*> params_types);
	FunctionTypeInfo(std::initializer_list<const Type*> init_list);

	std::string toString() const;

	long long hash_func() const;

	bool isCompatibleWith(const FunctionTypeInfo& info) const;

	int rankOfConversion(const FunctionTypeInfo& info) const;

	std::vector<const Type*> params_types;
};

bool operator==(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);
bool operator<(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);

#endif
