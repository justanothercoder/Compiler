#include "binaryoperatornode.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type) : lhs(lhs), rhs(rhs), op_type(op_type), resolved_operator_symbol(nullptr) { }

BinaryOperatorNode::~BinaryOperatorNode() { delete lhs; delete rhs; }

Type* BinaryOperatorNode::getType() const { return resolved_operator_symbol->getTypeInfo().return_type; }

void BinaryOperatorNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	lhs->check(template_sym, expr);
	resolved_operator_symbol = CallHelper::callCheck(getOperatorName(), static_cast<StructSymbol*>(TypeHelper::removeReference(lhs->getType())), {rhs}, template_sym, expr);
	
	if ( resolved_operator_symbol == nullptr )	
		resolved_operator_symbol = CallHelper::callCheck(getOperatorName(), getScope(), {lhs, rhs}, template_sym, expr);
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
    call_name += resolved_operator_symbol->getTypedName().substr(getCodeOperatorName().length());

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
	auto lhs_copy = static_cast<ExprNode*>(lhs->copyTree()), 
		 rhs_copy = static_cast<ExprNode*>(rhs->copyTree());

	return new BinaryOperatorNode(lhs_copy, rhs_copy,  op_type);    
}
	
vector<AST*> BinaryOperatorNode::getChildren() const { return {lhs, rhs}; }
