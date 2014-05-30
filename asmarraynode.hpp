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

	virtual void define();
	virtual void check();
	virtual CodeObject& gen();
	
	virtual AST* copyTree() const;

private:
	
	int size_of_type;
	int array_size;
	CodeObject code_obj;
};

#endif
