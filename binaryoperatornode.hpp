#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "exprnode.hpp"
#include "callinfo.hpp"

enum class BinaryOp { ASSIGN, PLUS, MINUS, MUL, EQUALS, NEQUALS, AND, OR, DIV, MOD };

class BinaryOperatorNode : public ExprNode
{
public:

    BinaryOperatorNode(ASTExprNode lhs, ASTExprNode rhs, BinaryOp op_type);

    std::string getOperatorName();
    std::string getCodeOperatorName();

    ASTChildren getChildren() const override;
    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* lhs();
    ExprNode* rhs();
    BinaryOp op() const;

    const CallInfo& callInfo() const;    
    void callInfo(const CallInfo& call_info);

protected:

    ASTExprNode lhs_;
    ASTExprNode rhs_;

    BinaryOp op_type;
    CallInfo call_info;
};

#endif
