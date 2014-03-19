#ifndef _CALLHELPER_HPP_
#define _CALLHELPER_HPP_

#include <vector>

#include "overloadedfunctionsymbol.hpp"
#include "exprnode.hpp"
#include "functionhelper.hpp"
#include "templatestructsymbol.hpp"
#include "callinfo.hpp"
#include "typehelper.hpp"

class CallHelper
{
public:

	static CallInfo callCheck(string name, Scope *sc, std::vector<ExprNode*> params, const TemplateInfo& template_info);
	static CallInfo getCallInfo(FunctionSymbol *function_sym, std::vector<ExprNode*> params_types);

	static OverloadedFunctionSymbol* getOverloadedFunc(string name, Scope *scope);
	static OverloadedFunctionSymbol* getOverloadedMethod(string name, StructSymbol *scope);

	static FunctionSymbol* resolveOverload(string name, Scope *sc, std::vector<VariableType> params_types);

private:
	
	static ConversionInfo getConversionInfo(VariableType lhs, VariableType rhs, bool is_lhs_left_value);
	static std::vector<VariableType> extractTypes(std::vector<ExprNode*> params);
};

#endif
