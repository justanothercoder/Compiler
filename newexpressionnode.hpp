#ifndef _NEWEXPRESSIONNODE_HPP_
#define _NEWEXPRESSIONNODE_HPP_

#include <vector>
#include <string>
#include <algorithm>

#include "exprnode.hpp"
#include "typeinfo.hpp"
#include "callinfo.hpp"
#include "inlineinfo.hpp"

class NewExpressionNode : public ExprNode
{
public:
    NewExpressionNode(TypeInfo type_info, std::vector<ASTExprNode> params);

    void build_scope() override;

    ASTChildren getChildren() const override;
    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const TypeInfo& typeInfo() const;
    void typeInfo(const TypeInfo& type_info);

    const std::vector<ASTExprNode>& params() const;

    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

    const InlineInfo& inlineInfo() const;
    void inlineInfo(InlineInfo inline_info);

private:

    TypeInfo type_info;
    std::vector<ASTExprNode> params_;

    CallInfo call_info;
    InlineInfo inline_info;
};

#endif
