#include "conversioninfo.hpp"
#include "functionsymbol.hpp"

ConversionInfo::ConversionInfo(FunctionSymbol *conversion, const Type* desired_type) : conversion(conversion)
                                                                                     , desired_type(desired_type)
{

}
