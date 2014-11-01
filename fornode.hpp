#ifndef _FORNODE_HPP_
#define _FORNODE_HPP_

#include <vector>
#include <memory>

#include "exprnode.hpp"

class ForNode : public AST
{

    friend class GenSSAVisitor;

public:

    ForNode(AST *init, ExprNode *cond, AST *step, AST *stats);

    void build_scope() override;

    AST* copyTree() const override;

    std::vector<AST*> getChildren() const override;
    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    AST *init;
    ExprNode *cond;
    AST *step;
    AST	*stats;

    std::shared_ptr<Scope> for_scope;
};

#endif
