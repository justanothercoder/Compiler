#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

#include "functionsymbol.hpp"

struct ConversionInfo 
{
	ConversionInfo(FunctionSymbol *conversion, bool deref, bool ref);	
	
	FunctionSymbol *conversion;
	bool deref, ref;
};

#endif
