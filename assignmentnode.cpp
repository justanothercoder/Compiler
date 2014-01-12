#include "assignmentnode.hpp"

AssignmentNode::AssignmentNode(ExprNode *lhs, ExprNode *rhs) : lhs(lhs), rhs(rhs)
{
    
}

void AssignmentNode::build_scope()
{
    lhs->scope = scope;
    lhs->build_scope();
    
    rhs->scope = scope;
    rhs->build_scope();
}

void AssignmentNode::define()
{
    
}

void AssignmentNode::check()
{
    lhs->check();
    rhs->check();

    if ( !lhs->isLeftValue() )
	throw SemanticError("left side of assignment is not a left value");

    Symbol *assignment = scope->resolve("operator=");

    if ( assignment == nullptr )
	throw SemanticError("No such operator =");

    if ( dynamic_cast<VariableSymbol*>(assignment) == nullptr )
	throw SemanticError("operator= is not a function or variable");
    
    OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(assignment)->getType());

    if ( ov_func == nullptr )
	throw SemanticError("error");
    
    auto overloads = FunctionHelper::getBestOverload(ov_func->getTypeInfo().overloads, {lhs->getType(), rhs->getType()});

    if ( overloads.empty() )
	throw SemanticError("No viable overload for operator= with " + lhs->getType()->getName() + " and " + rhs->getType()->getName() + " arguments.");

    resolved_function_symbol = ov_func->getTypeInfo().symbols[*std::begin(overloads)];
}

void AssignmentNode::gen()
{
    int paramsSize = 0;

    FunctionTypeInfo resolved_function_type_info = resolved_function_symbol->getTypeInfo();
    
    rhs->gen();
    if ( dynamic_cast<ReferenceType*>(resolved_function_type_info.getParamType(1)) )
    {
	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rax");
	paramsSize += sizeof(int*);
    }
    else
    {
	for ( int j = 0; j < resolved_function_type_info.getParamType(1)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
	{
	    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rbx");
	}
    }
    
    lhs->gen();
    if ( dynamic_cast<ReferenceType*>(resolved_function_type_info.getParamType(0)) )
    {
	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rax");
	paramsSize += sizeof(int*);
    }
    else
    {
	for ( int j = 0; j < resolved_function_type_info.getParamType(0)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
	{
	    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rbx");
	}
    }
    
    CodeGen::emit("sub rsp, " + std::to_string(paramsSize));

    string call_name = resolved_function_symbol->getEnclosingScope()->getScopeName() + "_";

    call_name += "~operator_assign";
    call_name += resolved_function_symbol->getTypedName().substr(resolved_function_symbol->getName().length());

    CodeGen::emit("call " + call_name);
    CodeGen::emit("add rsp, " + std::to_string(paramsSize));
}
