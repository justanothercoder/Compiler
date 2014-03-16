#include "callinfo.hpp"

CallInfo::CallInfo() : CallInfo(nullptr, { }, { }, { }) { }

CallInfo::CallInfo(FunctionSymbol *callee, const std::vector<Type*>& actual_params,
										   const std::vector<ConversionInfo>& conversions,
									 	   const std::vector<FunctionSymbol*>& copy_constructors
								) : callee(callee),
   									actual_params(actual_params),
									conversions(conversions),
									copy_constructors(copy_constructors)
{

}	
