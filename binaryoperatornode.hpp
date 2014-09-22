#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "exprnode.hpp"
#include "codeobject.hpp"

enum class BinaryOp { ASSIGN, PLUS, MINUS, MUL, EQUALS, NEQUALS, AND, OR, DIV, MOD };

class BinaryOperatorNode : public ExprNode
{
	friend class GenSSAVisitor;
	friend class CheckVisitor;

public:

    BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type);
	~BinaryOperatorNode() override;

    void check() override;
    CodeObject& gen() override;

	std::string getOperatorName();
	std::string getCodeOperatorName();

	std::vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    const Type* getType() const override;
	bool isLeftValue() const override;

	void freeTempSpace() override;
	
	bool isCompileTimeExpr() const override;
	boost::optional<int> getCompileTimeValue() const override;

	std::string toString() const override;
	
	void accept(ASTVisitor& visitor) override;

protected:

    ExprNode *lhs, *rhs;
    BinaryOp op_type;
    CallInfo call_info;
	CodeObject code_obj;
};

#endif
