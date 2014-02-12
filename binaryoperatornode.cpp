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
	lhs->check(template_sym, expr);
	rhs->check(template_sym, expr);

	special_check();

	Symbol *op_sym = this->getScope()->resolve(getOperatorName());

	OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(op_sym)->getType());

	resolved_operator_symbol = FunctionHelper::getViableOverload(ov_func, {lhs->getType(), rhs->getType()});

	if ( resolved_operator_symbol == nullptr )
		throw SemanticError("No viable overload for '" + getOperatorName() + "' with '" + lhs->getType()->getName() + "' and '" + rhs->getType()->getName() + "' arguments.");
}

void BinaryOperatorNode::special_check()
{
    switch ( op_type )
    {
    case BinaryOp::ASSIGN:
    {
	if ( !lhs->isLeftValue() )
	    throw SemanticError("left side of an assignment should be left value.");
    }
    case BinaryOp::PLUS:
    case BinaryOp::MINUS:
    case BinaryOp::MUL:
    {
	break;
    }
    }
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

    call_name += "~" + getCodeOperatorName();
    call_name += resolved_operator_symbol->getTypedName().substr(resolved_operator_symbol->getName().length());

    CodeGen::emit("push rsi");
    CodeGen::emit("lea rsi, [" + call_name + "]");

	if ( resolved_operator_symbol->isMethod() )
	{
		CodeGen::genCallCode(resolved_operator_symbol, {rhs}, template_sym, expr);
	}
	else
	    CodeGen::genCallCode(resolved_operator_symbol, {lhs, rhs}, template_sym, expr);

    CodeGen::emit("pop rsi");
}

AST* BinaryOperatorNode::copyTree() const
{
	return new BinaryOperatorNode(static_cast<ExprNode*>(lhs->copyTree()), static_cast<ExprNode*>(rhs->copyTree()), op_type);    
}
