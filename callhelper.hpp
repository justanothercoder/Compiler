#ifndef _CALLHELPER_HPP_
#define _CALLHELPER_HPP_

#include <string>
#include <vector>

#include "callinfo.hpp"

class Scope;
class ExprNode;
class StructSymbol;
class OverloadedFunctionSymbol;

class CallHelper
{
public:

    static CallInfo callCheck(std::string name, const Scope *sc, std::vector<ExprNode*> params);
    static CallInfo getCallInfo(const FunctionSymbol *function_sym, std::vector<ExprNode*> params_types);

    static const OverloadedFunctionSymbol* getOverloadedFunc(std::string name, const Scope *scope);
    static const OverloadedFunctionSymbol* getOverloadedMethod(std::string name, const StructSymbol *scope);

    static const FunctionSymbol* resolveOverload(std::string name, const Scope *sc, std::vector<const Type*> params_types);

    static ConversionInfo getConversionInfo(const Type *lhs, const Type *rhs, bool is_lhs_left_value);
    static std::vector<const Type*> extractTypes(std::vector<ExprNode*> params);
};

#endif
