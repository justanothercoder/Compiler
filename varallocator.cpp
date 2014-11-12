#include "varallocator.hpp"
#include "variablesymbol.hpp"
#include "type.hpp"
#include "globalconfig.hpp"

VarAllocator::VarAllocator() : space_for_variables(0)
{

}

void VarAllocator::addVariable(VariableSymbol *var)
{
    int type_size = var -> getType() -> getSize();

    var_addresses[var] = space_for_variables + GlobalConfig::int_size;
    space_for_variables += type_size;
}

int VarAllocator::getAddress(VariableSymbol *var) const
{
    return var_addresses.at(var);
}

int VarAllocator::getSpace() const
{
    return space_for_variables;
}
