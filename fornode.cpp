#include "fornode.hpp"
#include "localscope.hpp"

ForNode::ForNode(ASTNode init, ASTExprNode cond, ASTNode step, ASTNode stats) : init (std::move(init))
                                                                              , cond (std::move(cond))
                                                                              , step (std::move(step))
                                                                              , stats(std::move(stats))
{

}

void ForNode::build_scope()
{
    for_scope = std::make_shared<LocalScope>(scope.get());

    for ( auto child : getChildren() )
    {
        child -> scope = for_scope;
        child -> build_scope();
    }
}

ASTNode ForNode::copyTree() const
{
    return std::make_unique<ForNode>(
            init -> copyTree(), 
            ASTExprNode(static_cast<ExprNode*>(cond -> copyTree().release())),
            step -> copyTree(), 
            stats -> copyTree()
    );
}

std::vector<AST*> ForNode::getChildren() const { return {init.get(), cond.get(), step.get(), stats.get()}; }

std::string ForNode::toString() const
{
    return "for (" + init -> toString() + "; " + cond -> toString() + "; " + step -> toString() + ")\n" + stats -> toString();
}

void ForNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

AST* ForNode::initializer() { return init.get(); } 
ExprNode* ForNode::condition() { return cond.get(); } 
AST* ForNode::iteration() { return step.get(); }
AST* ForNode::body() { return stats.get(); }
