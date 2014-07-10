#include "overloadedfunctiontypeinfo.hpp"

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

	/*    
		  auto func_better = [&params_type](FunctionType *lhs, FunctionType *rhs)
		  {

		  }
	 */        
	return possible;
}
