#include "bracketnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"

BracketNode::BracketNode(ASTExprNode base, ASTExprNode expr) : base_(std::move(base)), expr_(std::move(expr)) { }

ASTNode BracketNode::copyTree() const
{
    return std::make_unique<BracketNode>(
               ASTExprNode(static_cast<ExprNode*>(base_ -> copyTree().release())),
               ASTExprNode(static_cast<ExprNode*>(expr_ -> copyTree().release()))
           );
}

ASTChildren BracketNode::getChildren() const { return {base_.get(), expr_.get()}; }

bool BracketNode::isCompileTimeExpr() const
{
    return base_ -> isCompileTimeExpr() 
        && expr_ -> isCompileTimeExpr() 
        && call_info.callee -> is_constexpr;
}

boost::optional<int> BracketNode::getCompileTimeValue() const { return boost::none; }

std::string BracketNode::toString() const { return base_ -> toString() + "[" + expr_ -> toString() + "]"; }

void BracketNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
ExprNode* BracketNode::base() { return base_.get(); }
ExprNode* BracketNode::expr() { return expr_.get(); }

const CallInfo& BracketNode::callInfo() const { return call_info; }
void BracketNode::callInfo(const CallInfo& call_info) { this -> call_info = call_info; }

const FunctionalType* BracketNode::function() const
{
    if ( base_ -> getType().unqualified() -> getTypeKind() == TypeKind::ARRAY )
        return BuiltIns::global_scope -> resolveFunction("operator[]");
    else
    {
        assert(base_ -> getType().unqualified() -> isObjectType());
        return static_cast<const ObjectType*>(base_ -> getType().unqualified()) -> resolveMethod("operator[]");
    }
}

std::vector<ValueInfo> BracketNode::arguments() const
{
    if ( base_ -> getType().unqualified() -> getTypeKind() == TypeKind::ARRAY )
        return {valueOf(base_.get()), valueOf(expr_.get())};
    else
        return {valueOf(expr_.get())};
}

