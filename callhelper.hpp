#ifndef _CALLHELPER_HPP_
#define _CALLHELPER_HPP_

#include <vector>

#include "overloadedfunctionsymbol.hpp"
#include "exprnode.hpp"
#include "functionhelper.hpp"
#include "callinfo.hpp"
#include "typehelper.hpp"

class CallHelper
{
public:

	static CallInfo callCheck(std::string name, const Scope *sc, std::vector<ExprNode*> params);
	static CallInfo getCallInfo(FunctionSymbol *function_sym, std::vector<ExprNode*> params_types);

	static OverloadedFunctionSymbol* getOverloadedFunc(string name, Scope *scope);
	static OverloadedFunctionSymbol* getOverloadedMethod(string name, StructSymbol *scope);

	static FunctionSymbol* resolveOverload(std::string name, const Scope *sc, std::vector<VariableType> params_types);

	static ConversionInfo getConversionInfo(VariableType lhs, VariableType rhs, bool is_lhs_left_value);
	static std::vector<VariableType> extractTypes(std::vector<ExprNode*> params);
};

#endif
