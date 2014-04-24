#include "varallocator.hpp"

VarAllocator::VarAllocator() : space_for_variables(0), space_for_locals(0), space_for_return_value(0), space_for_special(0) { }

void VarAllocator::addVariable(VariableSymbol *var) 
{
	int type_size = var->getType().getSize();

	var_addresses[var] = space_for_variables + GlobalConfig::int_size;
	space_for_variables += type_size;
}

void VarAllocator::addLocal(ExprNode *expr, int type_size) 
{ 
	locals[expr] = space_for_locals + GlobalConfig::int_size; 
	space_for_locals += type_size; 
}

int VarAllocator::getAddress(VariableSymbol *var) const 
{ 
	return var_addresses.at(var) + (!var->isParam() ? (space_for_locals + space_for_return_value + space_for_special) : 0);  
}

int VarAllocator::getAddress(ExprNode *expr) const { return locals.at(expr) + space_for_return_value + space_for_special; }

int VarAllocator::getSpace() const { return space_for_variables + space_for_locals + space_for_return_value + space_for_special; }

void VarAllocator::addReturnValueSpace(int size) { space_for_return_value += size; }

void VarAllocator::addSpecialSpace(ExprNode *expr) 
{ 
	special[expr] = GlobalConfig::int_size + space_for_special;
	space_for_special += GlobalConfig::int_size;
}
	
int VarAllocator::getAddressForLocal() const { return GlobalConfig::int_size; }
	
int VarAllocator::getSpecialAddress(ExprNode *expr) const { return special.at(expr) + space_for_return_value; }
