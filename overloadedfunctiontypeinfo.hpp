#ifndef _OVERLOADEDFUNCTIONTYPEINFO_HPP_
#define _OVERLOADEDFUNCTIONTYPEINFO_HPP_

#include <set>
#include <map>

#include "functiontypeinfo.hpp"

using std::set;
using std::map;

class FunctionSymbol;

class OverloadedFunctionTypeInfo
{
public:

    virtual string getName();
    virtual int getSize();
    
    OverloadedFunctionTypeInfo(const set<FunctionTypeInfo>& overloads);

    set<FunctionTypeInfo> overloads;
    map<FunctionTypeInfo, FunctionSymbol*> symbols;

};

bool operator<(const OverloadedFunctionTypeInfo& lhs, const OverloadedFunctionTypeInfo& rhs);

#endif
