#include "fornode.hpp"
#include "localscope.hpp"

ForNode::ForNode(AST *init, ExprNode *cond, AST *step, AST *stats) : init     (init)
                                                                   , cond     (cond)
                                                                   , step     (step)
                                                                   , stats    (stats)
{

}

void ForNode::build_scope()
{
    for_scope = std::make_shared<LocalScope>(scope);

    for ( auto child : getChildren() )
    {
        child -> scope = for_scope.get();
        child -> build_scope();
    }
}

AST* ForNode::copyTree() const
{
    return new ForNode(init -> copyTree(), 
                       static_cast<ExprNode*>(cond -> copyTree()), 
                       step -> copyTree(), 
                       stats -> copyTree()
    );
}

std::vector<AST*> ForNode::getChildren() const
{
    return {init, cond, step, stats};
}

std::string ForNode::toString() const
{
    return "for (" + init -> toString() + "; " + cond -> toString() + "; " + step -> toString() + ")\n" + stats -> toString();
}

void ForNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
