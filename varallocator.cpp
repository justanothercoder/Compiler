#include "varallocator.hpp"

VarAllocator::VarAllocator() : space_for_variables(0), space_for_locals(0) { }

void VarAllocator::addVariable(VariableSymbol *var) 
{
	int type_size = var->getType().getSize();

	var_addresses[var] = space_for_variables + GlobalConfig::int_size;
	space_for_variables += type_size;
}

void VarAllocator::addLocal(int type_size) { space_for_locals += type_size; }

int VarAllocator::getAddress(VariableSymbol *var) const { return var_addresses.at(var); }
int VarAllocator::getAddressForLocal() const { return space_for_variables + GlobalConfig::int_size; }
	
int VarAllocator::getSpace() const { return space_for_variables + space_for_locals; }
