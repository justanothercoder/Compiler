#ifndef _UNARYNODE_HPP_
#define _UNARYNODE_HPP_

#include "exprnode.hpp"
#include "callinfo.hpp"
#include "codeobject.hpp"

enum class UnaryOp { PLUS, MINUS, NOT };

class UnaryNode : public ExprNode
{

	friend class GenSSAVisitor;
	friend class CheckVisitor;

public:
	
	UnaryNode(ExprNode *exp, UnaryOp op_type);

	std::vector<AST*> getChildren() const override;
	AST* copyTree() const override;

	std::string getOperatorName();
	std::string getCodeOperatorName();
    
	const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

private:

	ExprNode *exp;
	UnaryOp op_type;
	CallInfo call_info;
	CodeObject code_obj;
};

#endif
