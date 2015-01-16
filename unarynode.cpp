#include "unarynode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"

UnaryNode::UnaryNode(ASTExprNode expr, UnaryOp op_type) : expr_(std::move(expr)), op_type(op_type) { }

std::string UnaryNode::getOperatorName() const
{
    switch ( op_type )
    {
        case UnaryOp::PLUS : return "operator+";
        case UnaryOp::MINUS: return "operator-";
        case UnaryOp::NOT  : return "operator!";
    }
}

std::string UnaryNode::getCodeOperatorName()
{
    switch ( op_type )
    {
        case UnaryOp::PLUS : return "operatorplus";
        case UnaryOp::MINUS: return "operatorminus";
        case UnaryOp::NOT  : return "operatornot";
    }
}

ASTChildren UnaryNode::getChildren() const { return {expr_.get()}; }

ASTNode UnaryNode::copyTree() const 
{ 
    return std::make_unique<UnaryNode>(ASTExprNode(static_cast<ExprNode*>(expr_ -> copyTree().release())), op_type); 
}

bool UnaryNode::isCompileTimeExpr() const { return expr_ -> isCompileTimeExpr() && call_info.callee -> is_constexpr; }
boost::optional<int> UnaryNode::getCompileTimeValue() const { return boost::none; }

std::string UnaryNode::toString() const
{
    std::string oper;

    switch ( op_type )
    {
        case UnaryOp::PLUS : oper = "+"; break;
        case UnaryOp::MINUS: oper = "-"; break;
        case UnaryOp::NOT  : oper = "!"; break;
    }

    return oper + expr_ -> toString();
}

void UnaryNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

ExprNode* UnaryNode::expr() { return expr_.get(); }

const CallInfo& UnaryNode::callInfo() const { return call_info; }
void UnaryNode::callInfo(const CallInfo& call_info) { this -> call_info = call_info; }
    
const FunctionalType* UnaryNode::function() const 
{
    assert(expr_ -> getType().unqualified() -> isObjectType());
    return static_cast<const ObjectType*>(expr_ -> getType().unqualified()) -> resolveMethod(getOperatorName());
}

std::vector<ValueInfo> UnaryNode::arguments() const { return { }; }

