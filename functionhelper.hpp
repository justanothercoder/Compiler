#ifndef _FUNCTIONHELPER_HPP_
#define _FUNCTIONHELPER_HPP_

#include <set>
#include <iostream>

#include "typehelper.hpp"
#include "functiontypeinfo.hpp"

using std::set;

class FunctionHelper
{
public:

    static bool isCompatible(FunctionTypeInfo ft, const vector<Type*>& params_type);
    static set<FunctionTypeInfo> getBestOverload(const set<FunctionTypeInfo>& selection, const vector<Type*>& params_type);
    static FunctionSymbol* getViableOverload(OverloadedFunctionSymbol* overloaded_func, const vector<Type*>& params_type);
    static void genCallCode(FunctionSymbol *func, const vector<ExprNode*>& params);
};   

#endif
