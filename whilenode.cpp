#include "whilenode.hpp"
#include "exprnode.hpp"
#include "localscope.hpp"

WhileNode::WhileNode(ASTExprNode cond, ASTNode stats) : cond(std::move(cond)), stats(std::move(stats))
{

}

void WhileNode::build_scope()
{
    while_scope = std::make_shared<LocalScope>(scope.get());

    cond -> scope = scope;
    cond -> build_scope();

    stats -> scope = while_scope;
    stats -> build_scope();
}

ASTNode WhileNode::copyTree() const
{
    return std::make_unique<WhileNode>(ASTExprNode(static_cast<ExprNode*>(cond -> copyTree().release())), stats -> copyTree());
}

std::vector<AST*> WhileNode::getChildren() const { return {cond.get(), stats.get()}; }
std::string WhileNode::toString() const { return "while (" + cond -> toString() + ")\n" + stats -> toString(); }

void WhileNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
ExprNode* WhileNode::condition() { return cond.get(); }
AST* WhileNode::body() { return stats.get(); }

