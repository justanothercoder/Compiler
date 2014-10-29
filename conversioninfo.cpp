#include "conversioninfo.hpp"
#include "functionsymbol.hpp"

ConversionInfo::ConversionInfo(FunctionSymbol *conversion, bool deref, bool ref) : conversion(conversion)
																				 , deref(deref)
																				 , ref(ref) 
																				 , actual_type(nullptr)
																				 , desired_type(nullptr)
{

}

bool ConversionInfo::operator==(const ConversionInfo& info) const
{
    auto h = std::hash<ConversionInfo>();
    return h(*this) == h(info);
}
