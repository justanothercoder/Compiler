#ifndef _UNARYNODE_HPP_
#define _UNARYNODE_HPP_

#include "exprnode.hpp"
#include "callinfo.hpp"
#include "functionsymbol.hpp"
#include "callhelper.hpp"
#include "codeobject.hpp"

enum class UnaryOp { PLUS, MINUS, NOT };

class UnaryNode : public ExprNode
{
public:
	
	UnaryNode(ExprNode *exp, UnaryOp op_type);
	virtual ~UnaryNode();

	virtual void check();
    virtual CodeObject& gen();

	virtual vector<AST*> getChildren() const;
	virtual AST* copyTree() const;

	string getOperatorName();
	string getCodeOperatorName();
    
	virtual VariableType getType() const;
	virtual bool isLeftValue() const;

private:

	ExprNode *exp;
	UnaryOp op_type;
	CallInfo call_info;
	CodeObject code_obj;
};

#endif
