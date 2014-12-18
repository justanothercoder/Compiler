#include "whilenode.hpp"
#include "exprnode.hpp"
#include "localscope.hpp"

WhileNode::WhileNode(ExprNode *cond, AST *stats) : cond(cond), stats(stats)
{

}

void WhileNode::build_scope()
{
    while_scope = std::make_shared<LocalScope>(scope);

    cond -> scope = scope;
    cond -> build_scope();

    stats -> scope = while_scope.get();
    stats -> build_scope();
}

AST* WhileNode::copyTree() const
{
    return new WhileNode(static_cast<ExprNode*>(cond -> copyTree()), stats -> copyTree());
}

std::vector<AST*> WhileNode::getChildren() const
{
    return {cond, stats};
}

std::string WhileNode::toString() const
{
    return "while (" + cond -> toString() + ")\n" + stats -> toString();
}

void WhileNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
