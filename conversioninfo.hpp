#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

class FunctionSymbol;

struct ConversionInfo 
{
	ConversionInfo(FunctionSymbol *conversion, bool pre_deref, bool post_ref);

	FunctionSymbol *conversion;
	bool pre_deref, post_ref;
};

#endif
