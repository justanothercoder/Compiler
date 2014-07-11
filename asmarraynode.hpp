#ifndef _ASMARRAYNODE_HPP_
#define _ASMARRAYNODE_HPP_

#include "codeobject.hpp"
#include "classvariablesymbol.hpp"
#include "templatesymbol.hpp"
#include "numbernode.hpp"
#include "functionsymboldefine.hpp"
#include "variablesymboldefine.hpp"

class AsmArrayNode : public AST
{
public:
	
	AsmArrayNode();

	void define() override;
	void check() override;

	CodeObject& gen() override;
	
	AST* copyTree() const override;

private:
	
	int size_of_type;
	int array_size;
	CodeObject code_obj;
};

#endif
