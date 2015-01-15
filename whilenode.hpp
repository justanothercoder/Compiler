#ifndef _WHILENODE_HPP_
#define _WHILENODE_HPP_

#include <memory>
#include "exprnode.hpp"

class WhileNode : public AST
{
public:

    WhileNode(ASTExprNode cond, ASTNode stats);
    void build_scope() override;

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* condition();
    AST* body();

private:

    ASTExprNode cond;
    ASTNode stats;

    std::shared_ptr<Scope> while_scope;
};

#endif
