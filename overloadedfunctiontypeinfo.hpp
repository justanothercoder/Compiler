#ifndef _OVERLOADEDFUNCTIONTYPEINFO_HPP_
#define _OVERLOADEDFUNCTIONTYPEINFO_HPP_

#include <set>
#include <map>

#include "functiontypeinfo.hpp"

class FunctionSymbol;

using std::set;
using std::map;

class OverloadedFunctionTypeInfo
{
public:

    string getName() const;
    int getSize() const;
    
    OverloadedFunctionTypeInfo(const set<FunctionTypeInfo>& overloads);    
    
    set<FunctionTypeInfo> overloads;
    map<FunctionTypeInfo, FunctionSymbol*> symbols;

};

bool operator<(const OverloadedFunctionTypeInfo& lhs, const OverloadedFunctionTypeInfo& rhs);

#endif
