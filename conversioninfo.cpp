#include "conversioninfo.hpp"
#include "functionsymbol.hpp"

ConversionInfo::ConversionInfo(FunctionSymbol *conversion) : conversion(conversion), desired_type(nullptr)
{

}
/*
bool ConversionInfo::operator<(const ConversionInfo& info) const
{
    if ( conversion == info.conversion )
        return desired_type < info.desired_type;
    
    return conversion < info.conversion;
}
*/
