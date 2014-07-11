#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

#include "variabletype.hpp"

class FunctionSymbol;

struct ConversionInfo 
{
	ConversionInfo(FunctionSymbol *conversion, bool deref, bool ref);	
	
	FunctionSymbol *conversion;
	bool deref, ref;

	VariableType actual_type;
	VariableType desired_type;
};

#endif
