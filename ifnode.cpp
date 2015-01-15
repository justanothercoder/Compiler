#include "ifnode.hpp"
#include "localscope.hpp"
#include "exprnode.hpp"

IfNode::IfNode(ASTExprNode cond, ASTNode stats_true, ASTNode stats_false) : cond       (std::move(cond))
                                                                          , stats_true (std::move(stats_true))
                                                                          , stats_false(std::move(stats_false))
{

}

void IfNode::build_scope()
{
    if_scope   = std::make_shared<LocalScope>(scope.get());
    else_scope = std::make_shared<LocalScope>(scope.get());

    cond -> scope = scope;
    cond -> build_scope();

    stats_true -> scope = if_scope;
    stats_true -> build_scope();

    stats_false -> scope = else_scope;
    stats_false -> build_scope();
}

ASTNode IfNode::copyTree() const
{
    return std::make_unique<IfNode>(ASTExprNode(static_cast<ExprNode*>(cond -> copyTree().release()))
                                  , stats_true -> copyTree()
                                  , stats_false -> copyTree());
}

ASTChildren IfNode::getChildren() const { return {cond.get(), stats_true.get(), stats_false.get()}; }

std::string IfNode::toString() const
{
    auto res = "if (" + cond -> toString() + ")\n";

    res += stats_true -> toString();

    if ( stats_false )
        res += "else\n" + stats_false -> toString();

    return res;
}

void IfNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
ExprNode* IfNode::condition() { return cond.get(); }
AST* IfNode::trueBranch()     { return stats_true.get(); }
AST* IfNode::falseBranch()    { return stats_false.get(); } 

