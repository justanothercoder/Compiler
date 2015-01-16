#ifndef _UNARYNODE_HPP_
#define _UNARYNODE_HPP_

#include "callablenode.hpp"

enum class UnaryOp { PLUS, MINUS, NOT };

class UnaryNode : public CallableNode
{
public:

    UnaryNode(ASTExprNode expr, UnaryOp op_type);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string getOperatorName() const;
    std::string getCodeOperatorName();

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* expr();

    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

    const FunctionalType* function() const override;
    std::vector<ValueInfo> arguments() const override;

private:

    ASTExprNode expr_;
    UnaryOp op_type;
};

#endif
