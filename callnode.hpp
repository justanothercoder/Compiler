#ifndef _CALLNODE_HPP_
#define _CALLNODE_HPP_

#include <vector>
#include <algorithm>
#include "callablenode.hpp"

class VariableSymbol;

class CallNode : public CallableNode
{
public:

    CallNode(ASTExprNode caller, std::vector<ASTExprNode> params);

    ASTChildren getChildren() const override;
    ASTNode copyTree() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* caller();
    const std::vector<ASTExprNode>& params() const;

    const FunctionalType* function() const override;
    std::vector<ValueInfo> arguments() const override;

private:

    ASTExprNode caller_;
    std::vector<ASTExprNode> params_;
};

#endif
