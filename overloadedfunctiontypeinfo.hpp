#ifndef _OVERLOADEDFUNCTIONTYPEINFO_HPP_
#define _OVERLOADEDFUNCTIONTYPEINFO_HPP_

#include <set>
#include <map>

#include "functiontypeinfo.hpp"

class FunctionSymbol;

class OverloadedFunctionTypeInfo
{
public:

    OverloadedFunctionTypeInfo(std::set<FunctionTypeInfo> overloads);
    
	std::set<FunctionTypeInfo> overloads;
	std::map<FunctionTypeInfo, FunctionSymbol*> symbols;

};

bool operator<(const OverloadedFunctionTypeInfo& lhs, const OverloadedFunctionTypeInfo& rhs);

#endif
