#include "conversioninfo.hpp"
#include "functionsymbol.hpp"

ConversionInfo::ConversionInfo(const FunctionSymbol *conversion, const Type* desired_type) : conversion(conversion)
                                                                                           , desired_type(desired_type)
{

}
    
std::string ConversionInfo::toString() const
{
    if ( conversion == nullptr )
        return "No conversion to '" + desired_type -> getName() + "'";
    else
        return "Conversion '" + conversion -> getName() + "' to '" + desired_type -> getName() + "'";
}
