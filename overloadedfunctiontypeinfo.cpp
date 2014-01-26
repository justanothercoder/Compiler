#include "overloadedfunctiontypeinfo.hpp"

OverloadedFunctionTypeInfo::OverloadedFunctionTypeInfo(const set<FunctionTypeInfo>& overloads) : overloads(overloads) { }
OverloadedFunctionTypeInfo::OverloadedFunctionTypeInfo(std::initializer_list<FunctionTypeInfo> il) : overloads(il) { }

string OverloadedFunctionTypeInfo::getName() const { return ""; }
int OverloadedFunctionTypeInfo::getSize() const { return sizeof(int*); }
