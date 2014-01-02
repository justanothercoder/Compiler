#include "assignmentnode.hpp"

AssignmentNode::AssignmentNode(ExprNode *lhs, ExprNode *rhs) : lhs(lhs), rhs(rhs)
{
    
}

void AssignmentNode::build_scope()
{
    lhs->scope = scope;
    rhs->scope = scope;
}

void AssignmentNode::define()
{
    
}

void AssignmentNode::check()
{
    lhs->check();
    rhs->check();

    if ( !lhs->isLeftValue() )
	throw;

    if ( lhs->getType() != rhs->getType() )
	throw;

    Symbol *assignment = scope->resolve("operator=");

    if ( assignment == nullptr || dynamic_cast<OverloadedFunctionType*>(static_cast<TypedSymbol*>(assignment)->getType()) == nullptr )
	throw;
    
    auto overloads = FunctionHelper::getBestOverload(static_cast<OverloadedFunctionType*>(static_cast<TypedSymbol*>(assignment)->getType())->overloads, {lhs->getType(), rhs->getType()});

    if ( overloads.empty() )
	throw;

    resolved_function_symbol = static_cast<OverloadedFunctionType*>(static_cast<TypedSymbol*>(assignment)->getType())->symbols[*std::begin(overloads)];
}

void AssignmentNode::gen()
{
    int paramsSize = 0;

    FunctionType *resolved_function_type = static_cast<FunctionType*>(resolved_function_symbol->getType());
    
    rhs->gen();
    for ( int j = 0; j < resolved_function_type->getParamType(1)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
    {
	CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize) + "], rbx");
    }
    
    lhs->gen();
    for ( int j = 0; j < resolved_function_type->getParamType(0)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
    {
	CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize) + "], rbx");
    }
    
    CodeGen::emit("sub rsp, " + std::to_string(paramsSize));

    string call_name = resolved_function_symbol->getEnclosingScope()->getScopeName() + "_";

    string operator_name = resolved_function_symbol->getName().substr(sizeof("operator") - 1);

    if ( operator_name != "=" )
	throw;

    operator_name = "@operator_assign";
    
    call_name += operator_name;
    
    call_name += resolved_function_symbol->getTypedName().substr(resolved_function_symbol->getName().length());

    CodeGen::emit("call " + call_name);
//    CodeGen::emit("call " + resolved_function_symbol->getEnclosingScope()->getScopeName() + "_" + resolved_function_symbol->getTypedName());
    CodeGen::emit("add rsp, " + std::to_string(paramsSize));
}
