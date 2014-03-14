#ifndef _CALLINFO_HPP_
#define _CALLINFO_HPP_

#include <vector>

#include "conversioninfo.hpp"

class FunctionSymbol;
class Type;

struct CallInfo
{
	CallInfo(FunctionSymbol *callee, const std::vector<Type*>& actual_params, const std::vector<ConversionInfo>& conversions);

	FunctionSymbol *callee;

	std::vector<Type*> actual_params;
	std::vector<ConversionInfo> conversions;
};

#endif
