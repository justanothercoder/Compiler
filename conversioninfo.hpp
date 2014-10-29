#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

#include <string>

class Type;
class FunctionSymbol;

struct ConversionInfo 
{
	ConversionInfo(FunctionSymbol *conversion, bool deref, bool ref);	

    bool operator==(const ConversionInfo& info) const;
	
	FunctionSymbol *conversion;
	bool deref, ref;

	const Type *actual_type;
	const Type *desired_type;
};

namespace std
{
    template<>
    struct hash<ConversionInfo>
    {
        typedef ConversionInfo argument_type;
        typedef std::size_t result_type;

        result_type operator() (const argument_type& s) const
        {
            return (reinterpret_cast<result_type>(s.conversion) << 32) + reinterpret_cast<result_type>(s.actual_type);
        }        
    };
}

#endif
