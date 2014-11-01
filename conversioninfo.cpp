#include "conversioninfo.hpp"
#include "functionsymbol.hpp"

ConversionInfo::ConversionInfo(FunctionSymbol *conversion, bool deref, bool ref) : conversion(conversion)
    , deref(deref)
    , ref(ref)
    , actual_type(nullptr)
    , desired_type(nullptr)
{

}

bool ConversionInfo::operator<(const ConversionInfo& info) const
{
    if ( conversion == info.conversion )
    {
        if ( deref == info.deref )
        {
            if ( ref == info.ref )
            {
                if ( actual_type == info.actual_type )
                    return desired_type < info.desired_type;

                return actual_type < info.actual_type;
            }
            
            return ref < info.ref;
        }
        
        return deref < info.deref;
    }
    
    return conversion < info.conversion;
}
