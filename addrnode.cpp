#include "addrnode.hpp"
#include "typefactory.hpp"
#include "type.hpp"
#include "globalconfig.hpp"
#include "semanticerror.hpp"
#include "scope.hpp"
#include "tempallocator.hpp"
#include "pointertype.hpp"
#include "globaltable.hpp"

AddrNode::AddrNode(ExprNode *expr, AddrOp op) : expr(expr), op(op)
{

}

VariableType AddrNode::getType() const
{
    if ( op == AddrOp::REF )
    {
        auto ptr = VariableType(TypeFactory::getPointer(expr -> getType().unqualified()), expr -> getType().isConst());
        return ptr;
    }
    else
    {
        auto type = expr -> getType();
        bool is_const = type.isConst();

        return VariableType(static_cast<const PointerType*>(type.unqualified()) -> getPointedType(), is_const);
    }
}

bool AddrNode::isLeftValue() const
{
    return (op == AddrOp::DEREF);
}

bool AddrNode::isCompileTimeExpr() const
{
    return false;
}

boost::optional<int> AddrNode::getCompileTimeValue() const
{
    return boost::none;
}

AST* AddrNode::copyTree() const
{
    return new AddrNode(static_cast<ExprNode*>(expr -> copyTree()), op);
}

std::vector<AST*> AddrNode::getChildren() const
{
    return {expr};
}

std::string AddrNode::toString() const
{
    return (op == AddrOp::REF ? std::string("&") : std::string("*")) + expr -> toString();
}

void AddrNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
