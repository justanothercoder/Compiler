#ifndef _VARALLOCATOR_HPP_
#define _VARALLOCATOR_HPP_

#include <map>

#include "variablesymbol.hpp"

class VarAllocator
{
public:
	VarAllocator();

	void addVariable(VariableSymbol *var);
	void addLocal(int type_size);

	int getAddress(VariableSymbol *var) const;
	int getAddressForLocal() const;

private:

	int space_for_variables;
	int space_for_locals;

	std::map<VariableSymbol*, int> var_addresses;
};

#endif
