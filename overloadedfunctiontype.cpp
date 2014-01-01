#include "overloadedfunctiontype.hpp"

OverloadedFunctionType::OverloadedFunctionType(const set<FunctionType*>& overloads) : overloads(overloads)
{
    
}

string OverloadedFunctionType::getName()
{
    return "";
}

int OverloadedFunctionType::getSize()
{
    return sizeof(int*);
}
