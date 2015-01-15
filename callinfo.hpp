#ifndef _CALLINFO_HPP_
#define _CALLINFO_HPP_

#include <vector>
#include <memory>
#include "conversioninfo.hpp"

class Type;
class FunctionSymbol;

struct CallInfo
{
    CallInfo();
    CallInfo(const FunctionSymbol* callee, std::vector<ConversionInfo> conversions);

    std::string toString() const;

    const FunctionSymbol* callee;
    std::vector<ConversionInfo> conversions;
};

#endif
