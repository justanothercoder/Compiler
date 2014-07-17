#ifndef _VARINFERTYPEDECLARATIONNODE_HPP_
#define _VARINFERTYPEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"
#include "exprnode.hpp"
#include "callinfo.hpp"
#include "codeobject.hpp"

class VariableSymbol;

class VarInferTypeDeclarationNode : public DeclarationNode
{
public:

	VarInferTypeDeclarationNode(string name, ExprNode *expr);

	void define() override;
	void check() override;
	CodeObject& gen() override;

	Symbol* getDefinedSymbol() const override;
	
	AST* copyTree() const override;	

	std::vector<AST*> getChildren() const override;

private:

	VariableSymbol *definedSymbol;

	string name;
	ExprNode *expr;

	CallInfo call_info;
	CodeObject code_obj;
};

#endif
