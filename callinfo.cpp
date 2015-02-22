#include "callinfo.hpp"
#include "functionsymbol.hpp"

CallInfo::CallInfo() : CallInfo(nullptr, { }) { }

CallInfo::CallInfo(const FunctionalSymbol* callee, std::vector<ConversionInfo> conversions) : callee(callee), conversions(conversions) { }
    
std::string CallInfo::toString() const
{
    std::string result = "";

    result += "Function: '" + callee -> getName() + "'\n";
    
    for ( auto info : conversions )
        result += info.toString() + "\n";

    return result;
}
