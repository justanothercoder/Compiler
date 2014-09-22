#ifndef _PARAMVARALLOCATOR_HPP_
#define _PARAMVARALLOCATOR_HPP_

#include "varallocator.hpp"

class ParamVarAllocator : public VarAllocator
{
public:

	ParamVarAllocator();

	void addVariable(VariableSymbol *var) override;

	int getSpaceForParams() const;

private:
	
	int space_for_params;
};

#endif
