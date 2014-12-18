#ifndef _NULLNODE_HPP_
#define _NULLNODE_HPP_

#include "exprnode.hpp"

class NullNode : public ExprNode
{
public:

    NullNode();

    AST* copyTree() const override;

    std::string toString() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    void accept(ASTVisitor& visitor) override;
};

#endif
