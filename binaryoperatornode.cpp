#include "binaryoperatornode.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type) : lhs(lhs), rhs(rhs), op_type(op_type), call_info() { }

BinaryOperatorNode::~BinaryOperatorNode() { delete lhs; delete rhs; }

VariableType BinaryOperatorNode::getType() const { return call_info.callee->getTypeInfo().return_type; }

void BinaryOperatorNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	lhs->check(template_sym, expr);
	call_info = CallHelper::callCheck(getOperatorName(), static_cast<StructSymbol*>(lhs->getType().type), {rhs}, template_sym, expr);
	
	if ( call_info.callee == nullptr )	
		call_info = CallHelper::callCheck(getOperatorName(), getScope(), {lhs, rhs}, template_sym, expr);
}

string BinaryOperatorNode::getOperatorName()
{
    switch ( op_type )
    {
    case BinaryOp::ASSIGN: return "operator=";
    case BinaryOp::PLUS  : return "operator+";
    case BinaryOp::MINUS : return "operator-";
    case BinaryOp::MUL   : return "operator*";
    }    
}

string BinaryOperatorNode::getCodeOperatorName()
{
    switch ( op_type )
    {
    case BinaryOp::ASSIGN: return "operatorassign";
    case BinaryOp::PLUS  : return "operatorplus";
    case BinaryOp::MINUS : return "operatorminus";
    case BinaryOp::MUL   : return "operatormul";
    }    
}

void BinaryOperatorNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( call_info.callee->isMethod() )
		CodeGen::genCallCode(call_info, {rhs}, template_sym, expr,
				[&]() { CodeGen::emit("lea rax, [" + call_info.callee->getScopedTypedName() + "]"); },
				[&]() { lhs->gen(template_sym, expr); }
		);
	else
		CodeGen::genCallCode(call_info, {lhs, rhs}, template_sym, expr,
				[&]() { CodeGen::emit("lea rax, [" + call_info.callee->getScopedTypedName() + "]"); },
				[](){}
		);
}

AST* BinaryOperatorNode::copyTree() const
{
	auto lhs_copy = static_cast<ExprNode*>(lhs->copyTree()), 
		 rhs_copy = static_cast<ExprNode*>(rhs->copyTree());

	return new BinaryOperatorNode(lhs_copy, rhs_copy,  op_type);    
}
	
vector<AST*> BinaryOperatorNode::getChildren() const { return {lhs, rhs}; }
