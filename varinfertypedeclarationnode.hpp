#ifndef _VARINFERTYPEDECLARATIONNODE_HPP_
#define _VARINFERTYPEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"
#include "callinfo.hpp"
#include "codeobject.hpp"

class ExprNode;
class VariableSymbol;

class VarInferTypeDeclarationNode : public DeclarationNode
{
public:

	VarInferTypeDeclarationNode(std::string name, ExprNode *expr);

	void define() override;
	void check() override;
	CodeObject& gen() override;

	Symbol* getDefinedSymbol() const override;
	
	AST* copyTree() const override;	

	std::vector<AST*> getChildren() const override;
	
	std::string toString() const override;
	
	void accept(ASTVisitor *visitor) override;

private:

	std::string name;
	ExprNode *expr;

	CallInfo call_info;
	CodeObject code_obj;
	
	VariableSymbol *definedSymbol;
};

#endif
