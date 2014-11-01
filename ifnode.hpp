#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include <memory>

#include "ast.hpp"

class Scope;
class ExprNode;

class IfNode : public AST
{
    friend class GenSSAVisitor;

public:

    IfNode(ExprNode *cond, AST *stats_true, AST *stats_false);

    AST* copyTree() const override;

    void build_scope() override;

    std::vector<AST*> getChildren() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    std::shared_ptr<Scope> if_scope;
    std::shared_ptr<Scope> else_scope;

    static std::string getNewLabel();
};

#endif
