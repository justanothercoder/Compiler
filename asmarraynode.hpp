#ifndef _ASMARRAYNODE_HPP_
#define _ASMARRAYNODE_HPP_

#include "ast.hpp"
#include "codeobject.hpp"

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
