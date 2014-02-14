#include "asmarraynode.hpp"


AsmArrayNode::AsmArrayNode(int size) : AsmNode(""), size(size)
{
	
}

void AsmArrayNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	CodeGen::emit("jmp _~_array_array_array~ref");
	CodeGen::emit("_array_array_array~ref:");
	CodeGen::emit("ret");
	CodeGen::emit("_~_array_array_array~ref:");
	CodeGen::emit("jmp _~_array_size_array~ref");
	CodeGen::emit("_array_size_array~ref:");

	CodeGen::emit("push rbp");
	CodeGen::emit("mov rbp, rsp");

	CodeGen::emit("mov rax, [rbp + 16]");
	CodeGen::emit("mov rax, [rax]");
	CodeGen::emit("mov [rsp - " + std::to_string(size) + ", rax");
	CodeGen::emit("lea rax, [rsp - " + std::to_string(size));

	CodeGen::emit("mov rsp, rbp");
	CodeGen::emit("pop rbp");
	CodeGen::emit("ret");

	CodeGen::emit("_~_array_size_array~ref:");

	CodeGen::emit("jmp _~_array_operatorelem_array~ref_int");
	CodeGen::emit("_array_operatorelem_array~ref_int:");
	CodeGen::emit("push rbp");
	CodeGen::emit("mov rbp, rsp");
	CodeGen::emit("mov rbx, [rbp + 24]");
	CodeGen::emit("imul rbx, " + std::to_string(size));

	CodeGen::emit("mov rax, [rbp + 16]");

	CodeGen::emit("sub rax, rbx");

	CodeGen::emit("mov rsp, rbp");
	CodeGen::emit("pop rbp");
	CodeGen::emit("ret");

	CodeGen::emit("_~_array_operatorelem_array~ref_int:");
}
