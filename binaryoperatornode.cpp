#include "binaryoperatornode.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type) : lhs(lhs), rhs(rhs), op_type(op_type) { resolved_operator_symbol = nullptr; }

BinaryOperatorNode::~BinaryOperatorNode() { delete lhs; delete rhs; }

bool BinaryOperatorNode::isLeftValue() const { return getType()->isReference(); }

Type* BinaryOperatorNode::getType() const { return resolved_operator_symbol->getTypeInfo().getReturnType(); }

void BinaryOperatorNode::build_scope()
{
    lhs->setScope(this->getScope());
    lhs->build_scope();
    
    rhs->setScope(this->getScope());
    rhs->build_scope();
}

void BinaryOperatorNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	auto ov_func = CallHelper::getOverloadedFunc(getOperatorName(), this->getScope());

	if ( ov_func->isMethod() )
	{
		lhs->check(template_sym, expr);
		resolved_operator_symbol = CallHelper::callCheck(ov_func, {rhs}, template_sym, expr);
	}
	else
		resolved_operator_symbol = CallHelper::callCheck(ov_func, {lhs, rhs}, template_sym, expr);
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
    string call_name = resolved_operator_symbol->getEnclosingScope()->getScopeName() + "_";

    call_name += getCodeOperatorName();
    call_name += resolved_operator_symbol->getTypedName().substr(resolved_operator_symbol->getName().length());

    CodeGen::emit("push rsi");
    CodeGen::emit("lea rsi, [" + call_name + "]");

	if ( resolved_operator_symbol->isMethod() )
	{
		lhs->gen(template_sym, expr);
		CodeGen::emit("push rdi");
		CodeGen::emit("mov rdi, rax");

		CodeGen::genCallCode(resolved_operator_symbol, {rhs}, template_sym, expr);
		CodeGen::emit("pop rdi");
	}
	else
	    CodeGen::genCallCode(resolved_operator_symbol, {lhs, rhs}, template_sym, expr);

    CodeGen::emit("pop rsi");
}

AST* BinaryOperatorNode::copyTree() const
{
	return new BinaryOperatorNode(static_cast<ExprNode*>(lhs->copyTree()), static_cast<ExprNode*>(rhs->copyTree()), op_type);    
}
	
vector<AST*> BinaryOperatorNode::getChildren() const { return {lhs, rhs}; }
