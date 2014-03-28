#ifndef _VARALLOCATOR_HPP_
#define _VARALLOCATOR_HPP_

#include <iostream>
#include <map>

#include "variablesymbol.hpp"
#include "globalconfig.hpp"

class VarAllocator
{
public:
	VarAllocator();

	virtual void addVariable(VariableSymbol *var);
	void addLocal(int type_size);

	int getAddress(VariableSymbol *var) const;
	int getAddressForLocal() const;

	int getSpace() const;
		
protected:

	int space_for_variables;
	int space_for_locals;

	std::map<VariableSymbol*, int> var_addresses;
};

#endif
