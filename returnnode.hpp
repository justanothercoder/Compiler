#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "codeobject.hpp"
#include "callinfo.hpp"
#include "ast.hpp"

class ExprNode;
class FunctionSymbol;

class ReturnNode : public AST
{
public:

    ReturnNode(ExprNode *expr);

	~ReturnNode() override;

    AST* copyTree() const override;

	void define() override;
	void check() override;

    CodeObject& gen() override;

	std::vector<AST*> getChildren() const override;

	std::string toString() const override;

private:

    ExprNode *expr;

	CallInfo copy_call_info;

	FunctionSymbol *enclosing_func;
	CodeObject code_obj;
};

#endif
