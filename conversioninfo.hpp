#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

#include <string>
#include <memory>

class Type;
class FunctionSymbol;

struct ConversionInfo
{
    ConversionInfo(const FunctionSymbol* conversion, const Type* desired_type = nullptr);

    std::string toString() const;

    const FunctionSymbol* conversion;
    const Type* desired_type;
};

#endif
