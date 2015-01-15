#include "bracketnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"

BracketNode::BracketNode(ASTExprNode base, ASTExprNode expr) : base_(std::move(base)), expr_(std::move(expr)) { }

ASTNode BracketNode::copyTree() const
{
    return std::make_unique<BracketNode>(
               ASTExprNode(static_cast<ExprNode*>(base_ -> copyTree().release())),
               ASTExprNode(static_cast<ExprNode*>(expr_ -> copyTree().release()))
           );
}

ASTChildren BracketNode::getChildren() const { return {base_.get(), expr_.get()}; }

VariableType BracketNode::getType() const { return call_info.callee -> type().returnType(); } 
bool BracketNode::isLeftValue() const { return false; }

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

