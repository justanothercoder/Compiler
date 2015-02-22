#ifndef _CONVERSIONINFO_HPP_
#define _CONVERSIONINFO_HPP_

#include <string>
#include <memory>

class Type;
class FunctionalSymbol;

struct ConversionInfo
{
    ConversionInfo(const FunctionalSymbol* conversion, const Type* desired_type = nullptr);

    std::string toString() const;

    const FunctionalSymbol* conversion;
    const Type* desired_type;
};

#endif
