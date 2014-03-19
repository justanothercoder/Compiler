#include "paramvarallocator.hpp"

ParamVarAllocator::ParamVarAllocator() : VarAllocator(), space_for_params(0) { }

void ParamVarAllocator::addVariable(VariableSymbol *var)
{
	if ( var->isParam() )
	{
		space_for_params += var->getType().getSize();
		var_addresses[var] = -(GlobalConfig::int_size + space_for_params);
	}
	else
		this->VarAllocator::addVariable(var);
}
	
int ParamVarAllocator::getSpaceForParams() const { return space_for_params; }
