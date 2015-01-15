#ifndef _OVERLOADEDFUNCTIONTYPEINFO_HPP_
#define _OVERLOADEDFUNCTIONTYPEINFO_HPP_

#include <set>
#include <map>
#include <algorithm>
#include <memory>
#include "functiontypeinfo.hpp"
#include "functionsymbol.hpp"

class OverloadedFunctionTypeInfo
{
public:

    OverloadedFunctionTypeInfo(std::set<FunctionTypeInfo> overloads);
    OverloadedFunctionTypeInfo(const OverloadedFunctionTypeInfo&) = delete;

    std::set<FunctionTypeInfo> getPossibleOverloads(FunctionTypeInfo params_type) const;

    std::set<FunctionTypeInfo> overloads;
    std::map<FunctionTypeInfo, std::shared_ptr<const Symbol> > symbols;

};

bool operator<(const OverloadedFunctionTypeInfo& lhs, const OverloadedFunctionTypeInfo& rhs);

#endif
