#ifndef _ADDRNODE_HPP_
#define _ADDRNODE_HPP_

#include <memory>
#include "exprnode.hpp"

enum class AddrOp { REF, DEREF };

class AddrNode : public ExprNode
{
public:

    AddrNode(ASTExprNode expr, AddrOp op);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* expr();
    AddrOp op() const;

private:

    ASTExprNode expr_;
    AddrOp op_;
};

#endif
