#include "binaryoperatornode.hpp"

BinaryOperatorNode::BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs) : lhs(lhs), rhs(rhs)
{
    
}

bool BinaryOperatorNode::isLeftValue()
{
    return (dynamic_cast<ReferenceType*>(getType()) != 0);
}

Type* BinaryOperatorNode::getType()
{
    return resolved_operator_symbol->getTypeInfo().getReturnType();
}

void BinaryOperatorNode::build_scope()
{
    lhs->scope = scope;
    lhs->build_scope();
    
    rhs->scope = scope;
    rhs->build_scope();
}

void BinaryOperatorNode::check()
{
    lhs->check();
    rhs->check();

    Symbol *op_sym = scope->resolve(getOperatorName());

    OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(op_sym)->getType());

    auto overloads = FunctionHelper::getBestOverload(ov_func->getTypeInfo().overloads, {lhs->getType(), rhs->getType()});

    if ( overloads.empty() )
	throw SemanticError("No viable overload for " + getOperatorName() + " with " + lhs->getType()->getName() + " and " + rhs->getType()->getName() + " arguments.");

    resolved_operator_symbol = ov_func->getTypeInfo().symbols[*std::begin(overloads)];
}
