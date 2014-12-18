#include "bracketnode.hpp"
#include "callhelper.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "globaltable.hpp"

BracketNode::BracketNode(ExprNode *base, ExprNode *expr) : base(base), expr(expr)
{

}

AST* BracketNode::copyTree() const
{
    return new BracketNode(
               static_cast<ExprNode*>(base -> copyTree()),
               static_cast<ExprNode*>(expr -> copyTree())
           );
}

std::vector<AST*> BracketNode::getChildren() const
{
    return {base, expr};
}

VariableType BracketNode::getType() const
{
    return call_info.callee -> type().returnType();
}

bool BracketNode::isLeftValue() const
{
    return false;
}

bool BracketNode::isCompileTimeExpr() const
{
    return base -> isCompileTimeExpr() && expr -> isCompileTimeExpr() && call_info.callee -> is_constexpr;
}

boost::optional<int> BracketNode::getCompileTimeValue() const
{
    return boost::none;
}

std::string BracketNode::toString() const
{
    return base -> toString() + "[" + expr -> toString() + "]";
}

void BracketNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
