#ifndef _FUNCTIONHELPER_HPP_
#define _FUNCTIONHELPER_HPP_

#include <set>

#include "typehelper.hpp"
#include "functiontypeinfo.hpp"

using std::set;

class FunctionHelper
{
public:

    static bool isCompatible(FunctionTypeInfo ft, const vector<Type*>& params_type);
    static set<FunctionTypeInfo> getBestOverload(const set<FunctionTypeInfo>& selection, const vector<Type*>& params_type);

};   

#endif
