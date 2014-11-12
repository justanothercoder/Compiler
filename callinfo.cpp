#include "callinfo.hpp"

CallInfo::CallInfo() : CallInfo(nullptr, { }) 
{

}

CallInfo::CallInfo(const FunctionSymbol *callee, std::vector<ConversionInfo> conversions) : callee(callee), conversions(conversions)
{

}
