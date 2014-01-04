#include "overloadedfunctiontypeinfo.hpp"

OverloadedFunctionTypeInfo::OverloadedFunctionTypeInfo(const set<FunctionTypeInfo>& overloads) : overloads(overloads)
{
    
}

string OverloadedFunctionTypeInfo::getName()
{
    return "";
}

int OverloadedFunctionTypeInfo::getSize()
{
    return sizeof(int*);
}
