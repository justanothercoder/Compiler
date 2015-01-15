#ifndef _FORNODE_HPP_
#define _FORNODE_HPP_

#include <vector>
#include <memory>

#include "exprnode.hpp"

class ForNode : public AST
{
public:

    ForNode(ASTNode init, ASTExprNode cond, ASTNode step, ASTNode stats);

    void build_scope() override;
    ASTNode copyTree() const override;

    ASTChildren getChildren() const override;
    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    AST* initializer();
    ExprNode* condition();
    AST* iteration();
    AST* body();    

private:

    ASTNode init;
    ASTExprNode cond;
    ASTNode step;
    ASTNode stats;

    std::shared_ptr<Scope> for_scope;
};

#endif
