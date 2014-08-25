#include "binaryoperatornode.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type) : lhs(lhs), rhs(rhs), op_type(op_type), call_info(), code_obj() { }

BinaryOperatorNode::~BinaryOperatorNode() { delete lhs; delete rhs; }

void BinaryOperatorNode::check()
{
	lhs -> check();
	try
	{
		call_info = CallHelper::callCheck(getOperatorName(), static_cast<const StructSymbol*>(lhs -> getType().type), {rhs});
	}
	catch ( SemanticError& e )
	{
		call_info = CallHelper::callCheck(getOperatorName(), scope, {lhs, rhs});
	}
	
	scope -> getTempAlloc().add(getType().getSize());
}

CodeObject& BinaryOperatorNode::gen()
{
	string addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(getType().getSize());

	code_obj.emit("lea rax, " + addr);
	code_obj.emit("push rax");

	if ( call_info.callee -> isMethod() )
		code_obj.genCallCode(call_info, {rhs}, lhs -> gen(), lhs -> getType().is_ref);
	else
	{
		CodeObject empty;
		code_obj.genCallCode(call_info, {lhs, rhs}, empty, false);
	}

	code_obj.emit("pop rax");

	return code_obj;
}

string BinaryOperatorNode::getOperatorName()
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

string BinaryOperatorNode::getCodeOperatorName()
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

AST* BinaryOperatorNode::copyTree() const
{
	auto lhs_copy = static_cast<ExprNode*>(lhs -> copyTree()), 
		 rhs_copy = static_cast<ExprNode*>(rhs -> copyTree());

	return new BinaryOperatorNode(lhs_copy, rhs_copy, op_type);
}

vector<AST*> BinaryOperatorNode::getChildren() const { return {lhs, rhs}; }

VariableType BinaryOperatorNode::getType() const { return call_info.callee -> return_type; }
bool BinaryOperatorNode::isLeftValue() const { return false; }

void BinaryOperatorNode::freeTempSpace()
{
	lhs -> freeTempSpace();
	rhs -> freeTempSpace();
	scope -> getTempAlloc().free();
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
