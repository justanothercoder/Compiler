#include "functionhelper.hpp"

bool FunctionHelper::isCompatible(FunctionTypeInfo ft, const vector<Type*>& params_type)
{
    if ( ft.getNumberOfParams() != static_cast<int>(params_type.size()) )
	return false;
       
    for ( int i = 0; i < static_cast<int>(params_type.size()); ++i )
    {
	if ( !TypeHelper::isConvertable(params_type[i], ft.getParamType(i)) )
	    return false;
    }

    return true;
}

set<FunctionTypeInfo> FunctionHelper::getBestOverload(const set<FunctionTypeInfo>& selection, const vector<Type*>& params_type)
{
    set<FunctionTypeInfo> possible = selection;

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

FunctionSymbol* FunctionHelper::getViableOverload(OverloadedFunctionSymbol* overloaded_func, const vector<Type*>& params_type)
{
    auto overloads = FunctionHelper::getBestOverload(overloaded_func->getTypeInfo().overloads, params_type);

    return overloads.empty() ? nullptr : overloaded_func->getTypeInfo().symbols[*std::begin(overloads)];   
}

