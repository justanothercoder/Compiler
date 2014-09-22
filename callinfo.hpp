#ifndef _CALLINFO_HPP_
#define _CALLINFO_HPP_

#include <vector>

#include "conversioninfo.hpp"

class Type;
class FunctionSymbol;

struct CallInfo
{
	CallInfo();

	CallInfo(const FunctionSymbol *callee, std::vector<const Type*> actual_params, 
									       std::vector<ConversionInfo> conversions,
									       std::vector<const FunctionSymbol*> copy_constructors
								);

	const FunctionSymbol *callee;

	std::vector<const Type*> actual_params;
	std::vector<ConversionInfo> conversions;
	std::vector<const FunctionSymbol*> copy_constructors;
};

#endif
