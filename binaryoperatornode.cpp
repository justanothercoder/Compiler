#include "binaryoperatornode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"

BinaryOperatorNode::BinaryOperatorNode(ASTExprNode lhs, ASTExprNode rhs, BinaryOp op_type) : lhs_(std::move(lhs))
                                                                                           , rhs_(std::move(rhs))
                                                                                           , op_type(op_type)
{

}

std::string BinaryOperatorNode::getOperatorName() const
{
    switch ( op_type )
    {
        case BinaryOp::ASSIGN : return "operator=" ;
        case BinaryOp::PLUS   : return "operator+" ;
        case BinaryOp::MINUS  : return "operator-" ;
        case BinaryOp::MUL    : return "operator*" ;
        case BinaryOp::DIV    : return "operator/" ;
        case BinaryOp::MOD    : return "operator%" ;
        case BinaryOp::EQUALS : return "operator==";
        case BinaryOp::NEQUALS: return "operator!=";
        case BinaryOp::AND    : return "operator&&";
        case BinaryOp::OR     : return "operator||";
    }
}

std::string BinaryOperatorNode::getCodeOperatorName()
{
    switch ( op_type )
    {
        case BinaryOp::ASSIGN : return "operatorassign";
        case BinaryOp::PLUS   : return "operatorplus"  ;
        case BinaryOp::MINUS  : return "operatorminus" ;
        case BinaryOp::MUL    : return "operatormul"   ;
        case BinaryOp::DIV    : return "operatordiv"   ;
        case BinaryOp::MOD    : return "operatormod"   ;
        case BinaryOp::EQUALS : return "operatoreq"    ;
        case BinaryOp::NEQUALS: return "operatorneq"   ;
        case BinaryOp::AND    : return "operatorand"   ;
        case BinaryOp::OR     : return "operatoror"    ;
    }
}

ASTNode BinaryOperatorNode::copyTree() const
{
    auto lhs_copy = ASTExprNode(static_cast<ExprNode*>(lhs_ -> copyTree().release())),
         rhs_copy = ASTExprNode(static_cast<ExprNode*>(rhs_ -> copyTree().release()));

    return std::make_unique<BinaryOperatorNode>(std::move(lhs_copy), std::move(rhs_copy), op_type);
}

ASTChildren BinaryOperatorNode::getChildren() const { return {lhs_.get(), rhs_.get()}; }

VariableType BinaryOperatorNode::getType() const { return call_info.callee -> type().returnType(); } 
bool BinaryOperatorNode::isLeftValue() const { return false; }

bool BinaryOperatorNode::isCompileTimeExpr() const
{
    return lhs_ -> isCompileTimeExpr() && rhs_ -> isCompileTimeExpr() && call_info.callee -> is_constexpr;
}

boost::optional<int> BinaryOperatorNode::getCompileTimeValue() const
{
    if ( !isCompileTimeExpr() )
        return boost::none;

    auto lhs_value = *lhs_ -> getCompileTimeValue();
    auto rhs_value = *rhs_ -> getCompileTimeValue();

    switch ( op_type )
    {
        case BinaryOp::PLUS   : return lhs_value + rhs_value;
        case BinaryOp::MINUS  : return lhs_value - rhs_value;
        case BinaryOp::MUL    : return lhs_value * rhs_value;
        case BinaryOp::DIV    : return lhs_value / rhs_value;
        case BinaryOp::MOD    : return lhs_value % rhs_value;
        case BinaryOp::EQUALS : return lhs_value == rhs_value;
        case BinaryOp::NEQUALS: return lhs_value != rhs_value;
        default: return boost::none;
    }
}

std::string BinaryOperatorNode::toString() const
{
    std::string oper;

    switch ( op_type )
    {
        case BinaryOp::ASSIGN : oper = "=" ; break;
        case BinaryOp::PLUS   : oper = "+" ; break;
        case BinaryOp::MINUS  : oper = "-" ; break;
        case BinaryOp::MUL    : oper = "*" ; break;
        case BinaryOp::EQUALS : oper = "=="; break;
        case BinaryOp::NEQUALS: oper = "!="; break;
        case BinaryOp::AND    : oper = "&&"; break;
        case BinaryOp::OR     : oper = "||"; break;
        case BinaryOp::DIV    : oper = "/" ; break;
        case BinaryOp::MOD    : oper = "%" ; break;
    }

    return lhs_ -> toString() + " " + oper + " " + rhs_ -> toString();
}

void BinaryOperatorNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
ExprNode* BinaryOperatorNode::lhs() { return lhs_.get(); }
ExprNode* BinaryOperatorNode::rhs() { return rhs_.get(); }
BinaryOp BinaryOperatorNode::op() const { return op_type; }

const CallInfo& BinaryOperatorNode::callInfo() const { return call_info; }
void BinaryOperatorNode::callInfo(const CallInfo& call_info) { this -> call_info = call_info; }

const FunctionalType* BinaryOperatorNode::function() const
{
    if ( lhs_ -> getType().unqualified() -> isObjectType() )
        return static_cast<const ObjectType*>(lhs_ -> getType().unqualified()) -> resolveMethod(getOperatorName());
    else
        return scope -> resolveFunction(getOperatorName());
}

std::vector<ValueInfo> BinaryOperatorNode::arguments() const 
{
    if ( lhs_ -> getType().unqualified() -> isObjectType() )
        return {valueOf(rhs_.get())};
    else
        return {valueOf(lhs_.get()), valueOf(rhs_.get())};
}
