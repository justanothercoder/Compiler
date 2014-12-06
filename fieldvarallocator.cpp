#include "fieldvarallocator.hpp"
#include "variablesymbol.hpp"
#include "type.hpp"

FieldVarAllocator::FieldVarAllocator() : VarAllocator()
{

}

void FieldVarAllocator::addVariable(VariableSymbol *var)
{
    int type_size = var -> getType().sizeOf();
    var_addresses[var] = space_for_variables;
    space_for_variables += type_size;
}
