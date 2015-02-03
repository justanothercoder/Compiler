#include "addrnode.hpp"
#include "typefactory.hpp"
#include "type.hpp"
#include "scope.hpp"
#include "pointertype.hpp"

AddrNode::AddrNode(ASTExprNode expr, AddrOp op) : expr_(std::move(expr)), op_(op) { }

VariableType AddrNode::getType() const
{
    auto type = expr_ -> getType();
    auto base_type = (op_ == AddrOp::REF ? TypeFactory::getPointer(type.unqualified()) 
                                         : static_cast<const PointerType*>(type.unqualified()) -> pointedType());

    return VariableType(base_type, type.isConst());
}

bool AddrNode::isLeftValue() const       { return (op_ == AddrOp::DEREF); }
bool AddrNode::isCompileTimeExpr() const { return false; }

boost::optional<int> AddrNode::getCompileTimeValue() const { return boost::none; }

ASTNode AddrNode::copyTree() const { return std::make_unique<AddrNode>(ASTExprNode(static_cast<ExprNode*>(expr_ -> copyTree().release())), op_); }
ASTChildren AddrNode::getChildren() const { return {expr_.get()}; }

std::string AddrNode::toString() const { return (op_ == AddrOp::REF ? std::string("&") : std::string("*")) + expr_ -> toString(); }

void AddrNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

ExprNode* AddrNode::expr() { return expr_.get(); }
AddrOp AddrNode::op() const { return op_; }
