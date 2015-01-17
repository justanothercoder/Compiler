#ifndef _NEWEXPRESSIONNODE_HPP_
#define _NEWEXPRESSIONNODE_HPP_

#include <vector>
#include <string>
#include <algorithm>

#include "exprnode.hpp"
#include "typeinfo.hpp"
#include "nodewithcall.hpp"

class NewExpressionNode : public ExprNode, public NodeWithCall
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

private:

    TypeInfo type_info;
    std::vector<ASTExprNode> params_;
};

#endif
