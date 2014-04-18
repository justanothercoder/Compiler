#include "binaryoperatornode.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type) : lhs(lhs), rhs(rhs), op_type(op_type), call_info(), code_obj() { }

BinaryOperatorNode::~BinaryOperatorNode() { delete lhs; delete rhs; }

void BinaryOperatorNode::check(const TemplateInfo& template_info)
{
	lhs->check(template_info);
	try
	{
		call_info = CallHelper::callCheck(getOperatorName(), static_cast<StructSymbol*>(lhs->getType().type), {rhs}, template_info);
	}
	catch ( SemanticError& e )
	{
		call_info = CallHelper::callCheck(getOperatorName(), getScope(), {lhs, rhs}, template_info);
	}
	
	getScope()->get_valloc()->addReturnValueSpace(getType().getSize());
}

CodeObject& BinaryOperatorNode::gen(const TemplateInfo& template_info)
{
	if ( call_info.callee->isMethod() )
		code_obj.genCallCode(call_info, {rhs}, template_info, lhs->gen(template_info), lhs->getType().is_ref);
	else
	{
		CodeObject empty;
		code_obj.genCallCode(call_info, {lhs, rhs}, template_info, empty, false);
	}

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
	auto lhs_copy = static_cast<ExprNode*>(lhs->copyTree()), 
		 rhs_copy = static_cast<ExprNode*>(rhs->copyTree());

	return new BinaryOperatorNode(lhs_copy, rhs_copy, op_type);
}

vector<AST*> BinaryOperatorNode::getChildren() const { return {lhs, rhs}; }

VariableType BinaryOperatorNode::getType() const { return call_info.callee->function_type_info.return_type; }
bool BinaryOperatorNode::isLeftValue() const { return false; }
