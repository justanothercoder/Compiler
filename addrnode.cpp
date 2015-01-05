#include "addrnode.hpp"
#include "typefactory.hpp"
#include "type.hpp"
#include "scope.hpp"
#include "pointertype.hpp"

AddrNode::AddrNode(ExprNode *expr, AddrOp op) : expr(expr), op(op) { }

VariableType AddrNode::getType() const
{
    auto type = expr -> getType();

    auto base_type = (op == AddrOp::REF ? TypeFactory::getPointer(type.unqualified()) : static_cast<const PointerType*>(type.unqualified()) -> pointedType());
    return VariableType(base_type, type.isConst());
}

bool AddrNode::isLeftValue() const       { return (op == AddrOp::DEREF); }
bool AddrNode::isCompileTimeExpr() const { return false; }

boost::optional<int> AddrNode::getCompileTimeValue() const { return boost::none; }

AST* AddrNode::copyTree() const { return new AddrNode(static_cast<ExprNode*>(expr -> copyTree()), op); }

std::vector<AST*> AddrNode::getChildren() const { return {expr}; }

std::string AddrNode::toString() const { return (op == AddrOp::REF ? std::string("&") : std::string("*")) + expr -> toString(); }

void AddrNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
