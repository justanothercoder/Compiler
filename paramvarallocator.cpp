#include "paramvarallocator.hpp"
#include "globalconfig.hpp"
#include "variablesymbol.hpp"
#include "type.hpp"

ParamVarAllocator::ParamVarAllocator(int offset) : VarAllocator(), space_for_params(0), offset(offset)
{

}

void ParamVarAllocator::addVariable(VariableSymbol *var)
{
    if ( var -> isParam() )
    {
        space_for_params += var -> getType() -> getSize();
//		var_addresses[var] = -(GlobalConfig::int_size + space_for_params);
//        var_addresses[var] = -(2 * GlobalConfig::int_size + space_for_params);
        var_addresses[var] = -(offset + space_for_params);
    }
    else
        this -> VarAllocator::addVariable(var);
}

int ParamVarAllocator::getSpaceForParams() const
{
    return space_for_params;
}
