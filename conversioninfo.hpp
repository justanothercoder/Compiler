#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

#include <string>

class Type;
class FunctionSymbol;

struct ConversionInfo
{
    ConversionInfo(FunctionSymbol *conversion);

    bool operator<(const ConversionInfo& info) const;

    FunctionSymbol *conversion;
    const Type *desired_type;
};

#endif
