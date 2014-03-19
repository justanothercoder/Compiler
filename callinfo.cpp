#include "callinfo.hpp"

CallInfo::CallInfo() : CallInfo(nullptr, { }, { }, { }) { }

CallInfo::CallInfo(FunctionSymbol *callee, std::vector<VariableType> actual_params,
										   std::vector<ConversionInfo> conversions,
									 	   std::vector<FunctionSymbol*> copy_constructors
								) : callee(callee),
   									actual_params(actual_params),
									conversions(conversions),
									copy_constructors(copy_constructors)
{

}	
