#ifndef _BRACKETNODE_HPP_
#define _BRACKETNODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

class BracketNode : public ExprNode
{
	friend class GenSSAVisitor;
	friend class CheckVisitor;

public:
	
	BracketNode(ExprNode *base, ExprNode *expr);
	~BracketNode() override;

	void check() override;
	CodeObject& gen() override;	

	AST* copyTree() const override;
	std::vector<AST*> getChildren() const override;

	const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor *visitor) override;

private:

	ExprNode *base, *expr;

	CallInfo call_info;
	CodeObject code_obj;
};

#endif
