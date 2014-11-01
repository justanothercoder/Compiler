#include "binaryoperatornode.hpp"
#include "functionsymbol.hpp"
#include "callhelper.hpp"
#include "structsymbol.hpp"
#include "globaltable.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type) : lhs(lhs), rhs(rhs), op_type(op_type)
{

}

std::string BinaryOperatorNode::getOperatorName()
{
    switch ( op_type )
    {
    case BinaryOp::ASSIGN :
        return "operator=" ;
    case BinaryOp::PLUS   :
        return "operator+" ;
    case BinaryOp::MINUS  :
        return "operator-" ;
    case BinaryOp::MUL    :
        return "operator*" ;
    case BinaryOp::DIV    :
        return "operator/" ;
    case BinaryOp::MOD    :
        return "operator%" ;
    case BinaryOp::EQUALS :
        return "operator==";
    case BinaryOp::NEQUALS:
        return "operator!=";
    case BinaryOp::AND    :
        return "operator&&";
    case BinaryOp::OR     :
        return "operator||";
    }
}

std::string BinaryOperatorNode::getCodeOperatorName()
{
    switch ( op_type )
    {
    case BinaryOp::ASSIGN :
        return "operatorassign";
    case BinaryOp::PLUS   :
        return "operatorplus"  ;
    case BinaryOp::MINUS  :
        return "operatorminus" ;
    case BinaryOp::MUL    :
        return "operatormul"   ;
    case BinaryOp::DIV    :
        return "operatordiv"   ;
    case BinaryOp::MOD    :
        return "operatormod"   ;
    case BinaryOp::EQUALS :
        return "operatoreq"    ;
    case BinaryOp::NEQUALS:
        return "operatorneq"   ;
    case BinaryOp::AND    :
        return "operatorand"   ;
    case BinaryOp::OR     :
        return "operatoror"    ;
    }
}

AST* BinaryOperatorNode::copyTree() const
{
    auto lhs_copy = static_cast<ExprNode*>(lhs -> copyTree()),
         rhs_copy = static_cast<ExprNode*>(rhs -> copyTree());

    return new BinaryOperatorNode(lhs_copy, rhs_copy, op_type);
}

std::vector<AST*> BinaryOperatorNode::getChildren() const
{
    return {lhs, rhs};
}

const Type* BinaryOperatorNode::getType() const
{
    return call_info.callee -> return_type;
}

bool BinaryOperatorNode::isLeftValue() const
{
    return false;
}

bool BinaryOperatorNode::isCompileTimeExpr() const
{
    return lhs -> isCompileTimeExpr() && rhs -> isCompileTimeExpr() && call_info.callee -> is_constexpr;
}

boost::optional<int> BinaryOperatorNode::getCompileTimeValue() const
{
    if ( !isCompileTimeExpr() )
        return boost::none;

    int lhs_value = *lhs -> getCompileTimeValue();
    int rhs_value = *rhs -> getCompileTimeValue();

    switch ( op_type )
    {
    case BinaryOp::PLUS   :
        return lhs_value + rhs_value;
    case BinaryOp::MINUS  :
        return lhs_value - rhs_value;
    case BinaryOp::MUL    :
        return lhs_value * rhs_value;
    case BinaryOp::DIV    :
        return lhs_value / rhs_value;
    case BinaryOp::MOD    :
        return lhs_value % rhs_value;
    case BinaryOp::EQUALS :
        return lhs_value == rhs_value;
    case BinaryOp::NEQUALS:
        return lhs_value != rhs_value;
    default:
        return boost::none;
    }
}

std::string BinaryOperatorNode::toString() const
{
    std::string lhs_str = lhs -> toString();
    std::string rhs_str = rhs -> toString();

    std::string oper;

    switch ( op_type )
    {
    case BinaryOp::ASSIGN :
        oper = "=" ;
        break;
    case BinaryOp::PLUS   :
        oper = "+" ;
        break;
    case BinaryOp::MINUS  :
        oper = "-" ;
        break;
    case BinaryOp::MUL    :
        oper = "*" ;
        break;
    case BinaryOp::EQUALS :
        oper = "==";
        break;
    case BinaryOp::NEQUALS:
        oper = "!=";
        break;
    case BinaryOp::AND    :
        oper = "&&";
        break;
    case BinaryOp::OR     :
        oper = "||";
        break;
    case BinaryOp::DIV    :
        oper = "/" ;
        break;
    case BinaryOp::MOD    :
        oper = "%" ;
        break;
    }

    return lhs_str + " " + oper + " " + rhs_str;
}

void BinaryOperatorNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
