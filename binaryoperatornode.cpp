#include "binaryoperatornode.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type) : lhs(lhs), rhs(rhs), op_type(op_type)
{
    
}

bool BinaryOperatorNode::isLeftValue() const
{
    return (dynamic_cast<ReferenceType*>(getType()) != 0);
}

Type* BinaryOperatorNode::getType() const
{
    return resolved_operator_symbol->getTypeInfo().getReturnType();
}

void BinaryOperatorNode::build_scope()
{
    lhs->setScope(getScope());
    lhs->build_scope();
    
    rhs->setScope(getScope());
    rhs->build_scope();
}

void BinaryOperatorNode::check()
{
    lhs->check();
    rhs->check();

    special_check();
    
    Symbol *op_sym = getScope()->resolve(getOperatorName());

    OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(op_sym)->getType());

    auto overloads = FunctionHelper::getBestOverload(ov_func->getTypeInfo().overloads, {lhs->getType(), rhs->getType()});

    if ( overloads.empty() )
	throw SemanticError("No viable overload for " + getOperatorName() + " with " + lhs->getType()->getName() + " and " + rhs->getType()->getName() + " arguments.");

    resolved_operator_symbol = ov_func->getTypeInfo().symbols[*std::begin(overloads)];
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

void BinaryOperatorNode::gen()
{
    int paramsSize = 0;

    FunctionTypeInfo resolved_operator_type_info = resolved_operator_symbol->getTypeInfo();

    auto params = {rhs, lhs};
    for ( int i = 1; i >= 0; --i )
    {    
	(*(std::begin(params) + i))->gen();
	if ( dynamic_cast<ReferenceType*>(resolved_operator_type_info.getParamType(i)) )
	{
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rax");
	    paramsSize += sizeof(int*);
	}
	else
	{
	    for ( int j = 0; j < resolved_operator_type_info.getParamType(1)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
	    {
		CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
		CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rbx");
	    }
	}
    }
    
    CodeGen::emit("sub rsp, " + std::to_string(paramsSize));

    string call_name = resolved_operator_symbol->getEnclosingScope()->getScopeName() + "_";

    call_name += "~" + getCodeOperatorName();
    call_name += resolved_operator_symbol->getTypedName().substr(resolved_operator_symbol->getName().length());

    CodeGen::emit("call " + call_name);
    CodeGen::emit("add rsp, " + std::to_string(paramsSize));
}
