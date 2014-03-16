#ifndef _CALLINFO_HPP_
#define _CALLINFO_HPP_

#include <vector>

#include "conversioninfo.hpp"

class FunctionSymbol;
class Type;

struct CallInfo
{
	CallInfo();

	CallInfo(FunctionSymbol *callee, const std::vector<Type*>& actual_params, 
									 const std::vector<ConversionInfo>& conversions,
									 const std::vector<FunctionSymbol*>& copy_constructors
								);

	FunctionSymbol *callee;

	std::vector<Type*> actual_params;
	std::vector<ConversionInfo> conversions;
	std::vector<FunctionSymbol*> copy_constructors;
};

#endif
