#ifndef _UNARYNODE_HPP_
#define _UNARYNODE_HPP_

#include "exprnode.hpp"
#include "callinfo.hpp"

enum class UnaryOp { PLUS, MINUS, NOT };

class UnaryNode : public ExprNode
{
public:

    UnaryNode(ASTExprNode expr, UnaryOp op_type);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string getOperatorName();
    std::string getCodeOperatorName();

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* expr();

    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

private:

    ASTExprNode expr_;
    UnaryOp op_type;
    CallInfo call_info;
};

#endif
