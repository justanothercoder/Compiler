#ifndef _FUNCTIONHELPER_HPP_
#define _FUNCTIONHELPER_HPP_

#include <set>

#include "typehelper.hpp"
#include "functiontype.hpp"

using std::set;

class FunctionHelper
{
public:

    static bool isCompatible(FunctionType *ft, const vector<Type*>& params_type);
    static set<FunctionType*> getBestOverload(const set<FunctionType*>& selection, const vector<Type*>& params_type);

};   

#endif
