#ifndef _WHILENODE_HPP_
#define _WHILENODE_HPP_

#include <memory>

#include "ast.hpp"

class Scope;
class ExprNode;

class WhileNode : public AST
{

    friend class GenSSAVisitor;

public:

    WhileNode(ExprNode *cond, AST *stats);

    AST* copyTree() const override;

    void build_scope() override;

    std::vector<AST*> getChildren() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    ExprNode *cond;
    AST *stats;

    std::shared_ptr<Scope> while_scope;

    static std::string getNewLabel();
};

#endif
