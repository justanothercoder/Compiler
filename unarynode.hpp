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
	~UnaryNode() override;

	void check() override;
    CodeObject& gen() override;

	vector<AST*> getChildren() const override;
	AST* copyTree() const override;

	string getOperatorName();
	string getCodeOperatorName();
    
	VariableType getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	optional<int> getCompileTimeValue() const override;

private:

	ExprNode *exp;
	UnaryOp op_type;
	CallInfo call_info;
	CodeObject code_obj;
};

#endif
