#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "callablenode.hpp"

enum class BinaryOp { ASSIGN, PLUS, MINUS, MUL, EQUALS, NEQUALS, AND, OR, DIV, MOD };

class BinaryOperatorNode : public CallableNode
{
public:

    BinaryOperatorNode(ASTExprNode lhs, ASTExprNode rhs, BinaryOp op_type);

    std::string getOperatorName() const;
    std::string getCodeOperatorName();

    ASTChildren getChildren() const override;
    ASTNode copyTree() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* lhs();
    ExprNode* rhs();
    BinaryOp op() const;

    const FunctionalType* function() const override;
    std::vector<ValueInfo> arguments() const override;

protected:

    ASTExprNode lhs_;
    ASTExprNode rhs_;

    BinaryOp op_type;
};

#endif
