#ifndef _CALLHELPER_HPP_
#define _CALLHELPER_HPP_

#include <vector>

#include "overloadedfunctionsymbol.hpp"
#include "exprnode.hpp"
#include "functionhelper.hpp"
#include "templatestructsymbol.hpp"

class CallHelper
{
public:

	static FunctionSymbol* callCheck(string name, Scope *sc, const std::vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*>& expr);
	static OverloadedFunctionSymbol* getOverloadedFunc(string name, Scope *scope);
	static OverloadedFunctionSymbol* getOverloadedMethod(string name, StructSymbol *scope);

	static FunctionSymbol* resolveOverload(string name, Scope *sc, std::vector<Type*> params_types);
	static FunctionSymbol* resolveOverload(string name, Scope *sc, std::vector<ExprNode*> params_types);
};

#endif
