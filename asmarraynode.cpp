#include "asmarraynode.hpp"
#include "templateinfo.hpp"
#include "functionsymbol.hpp"
#include "typefactory.hpp"
#include "templatesymbol.hpp"
#include "numbernode.hpp"
#include "builtins.hpp"
#include "globalhelper.hpp"

AsmArrayNode::AsmArrayNode() : size_of_type(0), array_size(0) 
{ 
//	scope = BuiltIns::global_scope; 
}

CodeObject& AsmArrayNode::gen()
{
//	auto arr = dynamic_cast<StructSymbol*>(scope);

	code_obj.emit("jmp _~" + array_constructor -> getScopedTypedName());
	code_obj.emit(array_constructor -> getScopedTypedName() + ":");
	code_obj.emit("ret");
	code_obj.emit("_~" + array_constructor -> getScopedTypedName() + ":");


/////////////////////////////////////////////////////////////////////////////
	code_obj.emit("jmp _~"  + array_size_func -> getScopedTypedName());
	code_obj.emit(array_size_func -> getScopedTypedName() + ":");

	code_obj.emit("push rbp");
	code_obj.emit("mov rbp, rsp");

	code_obj.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
	code_obj.emit("mov rax, [rax]");
	code_obj.emit("mov qword [rbp - " + std::to_string(array_size * size_of_type + GlobalConfig::int_size) + "], " + std::to_string(array_size));
	code_obj.emit("lea rax, [rbp - " + std::to_string(array_size * size_of_type + GlobalConfig::int_size) + "]");

	code_obj.emit("mov rsp, rbp");
	code_obj.emit("pop rbp");
	code_obj.emit("ret");

	code_obj.emit("_~" + array_size_func -> getScopedTypedName() + ":");
///////////////////////////////////////////////////////////////////////////////


	code_obj.emit("jmp _~" + array_elem_operator -> getScopedTypedName());
	code_obj.emit(array_elem_operator -> getScopedTypedName() + ":");

	code_obj.emit("push rbp");
	code_obj.emit("mov rbp, rsp");
	code_obj.emit("mov rbx, [rbp + " + std::to_string(3 * GlobalConfig::int_size) + "]");
	code_obj.emit("imul rbx, " + std::to_string(size_of_type));

	code_obj.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");

	code_obj.emit("sub rax, rbx");

	code_obj.emit("mov rbx, rax");
	code_obj.emit("mov rax, [rbp + " + std::to_string(4 * GlobalConfig::int_size) + "]");
	code_obj.emit("mov [rax], rbx");

	code_obj.emit("mov rsp, rbp");
	code_obj.emit("pop rbp");
	code_obj.emit("ret");

	code_obj.emit("_~" + array_elem_operator -> getScopedTypedName() + ":");

	return code_obj;
}
	
AST* AsmArrayNode::copyTree() const 
{
   	return new AsmArrayNode(*this); 
}
	
std::string AsmArrayNode::toString() const
{
	return "";
}

void AsmArrayNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
