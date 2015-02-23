#include "dotnode.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"

DotNode::DotNode(ASTExprNode base, const std::string& member_name) : base_(std::move(base)), member_name(member_name) { }

VariableType DotNode::getType() const
{
    return member_ -> isVariable() ? static_cast<const VarSymbol*>(member_) -> typeOf() 
                                   : VariableType(static_cast<const FunctionalSymbol*>(member_), true);
}

bool DotNode::isLeftValue() const { return true; }
ASTChildren DotNode::getChildren() const { return {base_.get()}; }

ASTNode DotNode::copyTree() const 
{
    return std::make_unique<DotNode>(ASTExprNode(static_cast<ExprNode*>(base_ -> copyTree().release())), member_name);
}

bool DotNode::isCompileTimeExpr() const { return false; }
boost::optional<int> DotNode::getCompileTimeValue() const { return boost::none; }

std::string DotNode::toString() const { return base_ -> toString() + "." + member_name; }
void DotNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
ExprNode* DotNode::base() { return base_.get(); }

const Symbol* DotNode::member() const { return member_; }
void DotNode::member(const Symbol* sym) { member_ = sym; }

const std::string& DotNode::memberName() const { return member_name; }

const ObjectType* DotNode::baseType() const { return base_type; }
void DotNode::baseType(const ObjectType* tp) { base_type = tp; }
