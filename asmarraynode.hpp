#ifndef _ASMARRAYNODE_HPP_
#define _ASMARRAYNODE_HPP_

#include "ast.hpp"
#include "codeobject.hpp"

class AsmArrayNode : public AST
{
public:
	
	AsmArrayNode();

	CodeObject& gen() override;
	
	AST* copyTree() const override;
	
	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

	int size_of_type;
	int array_size;
	CodeObject code_obj;
	
	FunctionSymbol *array_constructor;
	FunctionSymbol *array_elem_operator;
	FunctionSymbol *array_size_func;
};

#endif
