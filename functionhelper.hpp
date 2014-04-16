#ifndef _FUNCTIONHELPER_HPP_
#define _FUNCTIONHELPER_HPP_

#include <set>
#include <iostream>

#include "typehelper.hpp"
#include "functiontypeinfo.hpp"

#include "codeobject.hpp"

using std::set;

class FunctionHelper
{
public:

    static bool isCompatible(FunctionTypeInfo ft, vector<VariableType> params_type);
    static set<FunctionTypeInfo> getBestOverload(set<FunctionTypeInfo> selection, vector<VariableType> params_type);
    static FunctionSymbol* getViableOverload(OverloadedFunctionSymbol* overloaded_func, vector<VariableType> params_type);

	static FunctionSymbol* makeDefaultCopyConstructor(StructSymbol *struc, const TemplateInfo& template_info);
	static FunctionSymbol* makeDefaultConstructor(StructSymbol *struc, const TemplateInfo& template_info);
};   

#endif
