#ifndef _CALLNODE_HPP_
#define _CALLNODE_HPP_

#include <vector>
#include <algorithm>

#include "exprnode.hpp"
#include "callinfo.hpp"
#include "inlineinfo.hpp"

class VariableSymbol;

class CallNode : public ExprNode
{
public:

    CallNode(ASTExprNode caller, std::vector<ASTExprNode> params);

    ASTChildren getChildren() const override;
    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* caller();
    const std::vector<ASTExprNode>& params() const;

    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

    const InlineInfo& inlineInfo() const;
    void inlineInfo(InlineInfo inline_info);

private:

    ASTExprNode caller_;
    std::vector<ASTExprNode> params_;

    CallInfo call_info;
    InlineInfo inline_info;
};

#endif
