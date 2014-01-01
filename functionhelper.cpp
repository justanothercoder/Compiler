#include "functionhelper.hpp"

bool FunctionHelper::isCompatible(FunctionType *ft, const vector<Type*>& params_type)
{
    for ( int i = 0; i < ft->getNumberOfParams(); ++i )
    {
	if ( ft->getParamType(i) != params_type[i] )
	    return false;
    }

    return true;
}

set<FunctionType*> FunctionHelper::getBestOverload(const set<FunctionType*>& selection, const vector<Type*>& params_type)
{
    set<FunctionType*> possible = selection;

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
