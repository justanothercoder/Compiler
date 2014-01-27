#include "overloadedfunctiontypeinfo.hpp"

OverloadedFunctionTypeInfo::OverloadedFunctionTypeInfo(const set<FunctionTypeInfo>& overloads) : overloads(overloads) { }

string OverloadedFunctionTypeInfo::getName() const { return ""; }
int OverloadedFunctionTypeInfo::getSize() const { return sizeof(int*); }
