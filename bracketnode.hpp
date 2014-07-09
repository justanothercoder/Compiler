#ifndef _BRACKETNODE_HPP_
#define _BRACKETNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "functionhelper.hpp"
#include "codeobject.hpp"
#include "callhelper.hpp"

class BracketNode : public ExprNode
{
public:
	
	BracketNode(ExprNode *base, ExprNode *expr);
	~BracketNode() override;

	void check() override;
	CodeObject& gen() override;	

	AST* copyTree() const override;
	vector<AST*> getChildren() const override;

	VariableType getType() const override;
	bool isLeftValue() const override;

	int neededSpaceForTemporaries() override;

	void freeTempSpace() override;

private:

	ExprNode *base, *expr;

	CallInfo call_info;
	CodeObject code_obj;
};

#endif
