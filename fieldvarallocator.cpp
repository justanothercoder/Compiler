#include "fieldvarallocator.hpp"

FieldVarAllocator::FieldVarAllocator() : VarAllocator() { }

void FieldVarAllocator::addVariable(VariableSymbol *var) 
{
	int type_size = var->getType()->getSize();
	space_for_variables += type_size;
	var_addresses[var] = space_for_variables - GlobalConfig::int_size;
}
