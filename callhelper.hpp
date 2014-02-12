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

	static FunctionSymbol* callCheck(OverloadedFunctionSymbol *ov_func, const std::vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*>& expr);
};

#endif
