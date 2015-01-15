#include "returnnode.hpp"
#include "functionsymbol.hpp"
#include "exprnode.hpp"

ReturnNode::ReturnNode(ASTExprNode expr) : expr_(std::move(expr)) { }

ASTNode ReturnNode::copyTree() const { return std::make_unique<ReturnNode>(ASTExprNode(static_cast<ExprNode*>(expr_ -> copyTree().release()))); } 
ASTChildren ReturnNode::getChildren() const { return {expr_.get()}; } 

std::string ReturnNode::toString() const { return "return " + expr_ -> toString() + ";"; } 
void ReturnNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
ExprNode* ReturnNode::expr() { return expr_.get(); }
bool ReturnNode::isInInlineCall() const { return is_in_inline_call; }

const FunctionSymbol* ReturnNode::function() const { return enclosing_func; }
void ReturnNode::function(const FunctionSymbol* func) { enclosing_func = func; }

