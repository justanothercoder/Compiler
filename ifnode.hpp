#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include <memory>
#include "exprnode.hpp"

class Scope;

class IfNode : public AST
{
public:

    IfNode(ASTExprNode cond, ASTNode stats_true, ASTNode stats_false);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    void build_scope() override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* condition();
    AST* trueBranch();
    AST* falseBranch();

private:

    ASTExprNode cond;
    ASTNode stats_true; 
    ASTNode stats_false;

    std::shared_ptr<Scope> if_scope;
    std::shared_ptr<Scope> else_scope;
};

#endif
