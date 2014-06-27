#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "codeobject.hpp"
#include "callinfo.hpp"

class ReturnNode : public AST
{
public:

    ReturnNode(ExprNode *expr);
	
	~ReturnNode() override;

    AST* copyTree() const override;

	void define() override;
	void check() override;

    CodeObject& gen() override;

	vector<AST*> getChildren() const override;

	int neededSpaceForTemporaries() override;

private:

    ExprNode *expr;

	CallInfo copy_call_info;

	FunctionSymbol *enclosing_func;
	CodeObject code_obj;
};

#endif
