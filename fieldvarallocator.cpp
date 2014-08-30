#include "fieldvarallocator.hpp"

FieldVarAllocator::FieldVarAllocator() : VarAllocator() { }

void FieldVarAllocator::addVariable(VariableSymbol *var) 
{
	int type_size = var -> getType() -> getSize();
	var_addresses[var] = space_for_variables;
	space_for_variables += type_size;
}
