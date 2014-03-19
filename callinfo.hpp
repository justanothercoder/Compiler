#ifndef _CALLINFO_HPP_
#define _CALLINFO_HPP_

#include <vector>

#include "conversioninfo.hpp"
#include "variabletype.hpp"

class FunctionSymbol;

struct CallInfo
{
	CallInfo();

	CallInfo(FunctionSymbol *callee, std::vector<VariableType> actual_params, 
									 std::vector<ConversionInfo> conversions,
									 std::vector<FunctionSymbol*> copy_constructors
								);

	FunctionSymbol *callee;

	std::vector<VariableType> actual_params;
	std::vector<ConversionInfo> conversions;
	std::vector<FunctionSymbol*> copy_constructors;
};

#endif
