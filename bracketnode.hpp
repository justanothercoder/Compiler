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
	virtual ~BracketNode();

	virtual void check();
	virtual CodeObject& gen();	

	virtual AST* copyTree() const;
	virtual vector<AST*> getChildren() const;

	virtual VariableType getType() const;
	virtual bool isLeftValue() const;

	virtual int neededSpaceForTemporaries();

private:

	ExprNode *base, *expr;

	CallInfo call_info;
	CodeObject code_obj;
};

#endif
