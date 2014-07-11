#include "overloadedfunctiontypeinfo.hpp"
	
//#include "logger.hpp"

OverloadedFunctionTypeInfo::OverloadedFunctionTypeInfo(std::set<FunctionTypeInfo> overloads) : overloads(overloads) { }
	
std::set<FunctionTypeInfo> OverloadedFunctionTypeInfo::getBestOverload(FunctionTypeInfo params_type) const
{
	auto possible = overloads;

	for ( auto i = std::begin(possible); i != std::end(possible); )
	{
		if ( !i -> isCompatibleWith(params_type) )
			i = possible.erase(i);
		else
			++i;
	}
	    
    auto func_better = [&params_type](const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs)
    {
		return lhs.rankOfConversion(params_type) < rhs.rankOfConversion(params_type);
    };

	std::vector<FunctionTypeInfo> v(std::begin(possible), std::end(possible));
	
	std::sort(std::begin(v), std::end(v), func_better);
/*
	Logger::log("Debug: " + params_type.toString());
	for ( auto ft : v )
		Logger::log(ft.toString() + " " + std::to_string(ft.rankOfConversion(params_type)));
*/
//	return possible;
	return { v.front() };
}
