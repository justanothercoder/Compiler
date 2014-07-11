#ifndef _VARALLOCATOR_HPP_
#define _VARALLOCATOR_HPP_

#include <iostream>
#include <map>

#include "variablesymbol.hpp"
#include "globalconfig.hpp"

#include "exprnode.hpp"

class VarAllocator
{
public:
	VarAllocator();

	virtual void addVariable(VariableSymbol *var);
	
	int getAddress(VariableSymbol *var) const;

	int getSpace() const;
		
protected:

	int space_for_variables;

	std::map<VariableSymbol*, int> var_addresses;
};

#endif
