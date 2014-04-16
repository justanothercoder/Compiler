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
	
	void addLocal(ExprNode *expr, int type_size);
	void addReturnValueSpace(int size);
	
	int getAddress(VariableSymbol *var) const;
	int getAddress(ExprNode *expr) const;

	int getAddressForLocal() const;

	int getSpace() const;
		
protected:

	int space_for_variables;
	int space_for_locals;
	int space_for_return_value;

	std::map<VariableSymbol*, int> var_addresses;
	std::map<ExprNode*, int> locals;
};

#endif
