#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

#include <string>

class Type;
class FunctionSymbol;

struct ConversionInfo
{
    ConversionInfo(FunctionSymbol *conversion, bool deref, bool ref);

    bool operator<(const ConversionInfo& info) const;

    FunctionSymbol *conversion;
    bool deref, ref;

    const Type *actual_type;
    const Type *desired_type;
};

#endif
