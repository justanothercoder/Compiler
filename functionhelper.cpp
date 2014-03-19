#include "functionhelper.hpp"

bool FunctionHelper::isCompatible(FunctionTypeInfo ft, vector<VariableType> params_type)
{
	if ( ft.params_types.size() != params_type.size() )
		return false;	

	for ( size_t i = 0; i < params_type.size(); ++i )
	{
		if ( !TypeHelper::isConvertable(params_type[i], ft.params_types[i]) )
			return false;
	}

	return true;
}

set<FunctionTypeInfo> FunctionHelper::getBestOverload(set<FunctionTypeInfo> selection, vector<VariableType> params_type)
{
	auto possible = selection;

	for ( auto i = std::begin(possible); i != std::end(possible); )
	{
		if ( !isCompatible(*i, params_type) )
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

FunctionSymbol* FunctionHelper::getViableOverload(OverloadedFunctionSymbol *overloaded_func, vector<VariableType> params_type)
{
	auto overloads = FunctionHelper::getBestOverload(overloaded_func->getTypeInfo().overloads, params_type);

	return overloads.empty() ? nullptr : overloaded_func->getTypeInfo().symbols[*std::begin(overloads)];   
}
