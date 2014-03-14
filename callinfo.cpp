#include "callinfo.hpp"

CallInfo::CallInfo(FunctionSymbol *callee, const std::vector<Type*>& actual_params, 
										   const std::vector<ConversionInfo>& conversions) : callee(callee),
																							 actual_params(actual_params),
																							 conversions(conversions)
{

}	
