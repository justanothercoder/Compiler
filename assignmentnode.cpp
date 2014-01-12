#include "assignmentnode.hpp"

AssignmentNode::AssignmentNode(ExprNode *lhs, ExprNode *rhs) : BinaryOperatorNode(lhs, rhs)
{
    
}

string AssignmentNode::getOperatorName()
{
    return "operator=";
}

void AssignmentNode::gen()
{
    int paramsSize = 0;

    FunctionTypeInfo resolved_operator_type_info = resolved_operator_symbol->getTypeInfo();
    
    rhs->gen();
    if ( dynamic_cast<ReferenceType*>(resolved_operator_type_info.getParamType(1)) )
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
    
    lhs->gen();
    if ( dynamic_cast<ReferenceType*>(resolved_operator_type_info.getParamType(0)) )
    {
	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rax");
	paramsSize += sizeof(int*);
    }
    else
    {
	for ( int j = 0; j < resolved_operator_type_info.getParamType(0)->getSize(); j += sizeof(int*), paramsSize += sizeof(int*) )
	{
	    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + sizeof(int*)) + "], rbx");
	}
    }
    
    CodeGen::emit("sub rsp, " + std::to_string(paramsSize));

    string call_name = resolved_operator_symbol->getEnclosingScope()->getScopeName() + "_";

    call_name += "~operator_assign";
    call_name += resolved_operator_symbol->getTypedName().substr(resolved_operator_symbol->getName().length());

    CodeGen::emit("call " + call_name);
    CodeGen::emit("add rsp, " + std::to_string(paramsSize));
}
