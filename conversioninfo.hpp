#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

class Type;
class FunctionSymbol;

struct ConversionInfo 
{
	ConversionInfo(FunctionSymbol *conversion, bool deref, bool ref);	
	
	FunctionSymbol *conversion;
	bool deref, ref;

	const Type *actual_type;
	const Type *desired_type;
};

#endif
