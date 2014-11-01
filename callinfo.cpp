#include "callinfo.hpp"

CallInfo::CallInfo() : CallInfo(nullptr, { }, { }, { }) { }

CallInfo::CallInfo(const FunctionSymbol *callee, std::vector<const Type*> actual_params,
                   std::vector<ConversionInfo> conversions,
                   std::vector<const FunctionSymbol*> copy_constructors
                  ) : callee(callee),
    actual_params(actual_params),
    conversions(conversions),
    copy_constructors(copy_constructors)
{

}
