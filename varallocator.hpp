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

	void addSpecialSpace(ExprNode *expr);

	int getAddress(VariableSymbol *var) const;
	int getAddress(ExprNode *expr) const;

	int getSpecialAddress(ExprNode *expr) const;

	int getAddressForLocal() const;

	int getSpace() const;
		
protected:

	int space_for_variables;
	int space_for_locals;
	int space_for_return_value;
	int space_for_special;

	std::map<VariableSymbol*, int> var_addresses;
	std::map<ExprNode*, int> locals;
	std::map<ExprNode*, int> special;
};

#endif
