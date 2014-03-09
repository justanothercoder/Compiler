#ifndef _PARAMVARALLOCATOR_HPP_
#define _PARAMVARALLOCATOR_HPP_

#include "varallocator.hpp"
#include "globalconfig.hpp"

class ParamVarAllocator : public VarAllocator
{
public:

	ParamVarAllocator();

	virtual void addVariable(VariableSymbol *var);

	int getSpaceForParams() const;

private:
	
	int space_for_params;
};

#endif
