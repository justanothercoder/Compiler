#include "asmarraynode.hpp"

AsmArrayNode::AsmArrayNode() : size_of_type(0), array_size(0) { setScope(BuiltIns::global_scope); }

void AsmArrayNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) 
{
	Type *type = nullptr, *ref_type = nullptr;

	if ( template_sym->isIn("size") )
	{
		auto replace = template_sym->getReplacement("size", expr);
		
		array_size = std::stoi(dynamic_cast<NumberNode*>(replace)->getNum());
	}
	else throw SemanticError("");

	if ( template_sym->isIn("T") )
	{
		auto replace = template_sym->getReplacement("T", expr);

		type = static_cast<ClassVariableSymbol*>(TypeHelper::removeReference(replace->getType()))->sym;
		ref_type = TypeHelper::getReferenceType(type);
		size_of_type = type->getSize();
	}
	else throw SemanticError("");

	auto arr = dynamic_cast<StructSymbol*>(getScope());
	auto ref_arr = TypeHelper::getReferenceType(arr);

	auto array_constructor = new FunctionSymbol(
			"array",			
			FunctionTypeInfo(ref_arr, {ref_arr}),
			getScope(),
			{true, true, false}
			);

	auto array_elem_operator = new FunctionSymbol(
			"operator[]",
			FunctionTypeInfo(ref_type, {ref_arr, BuiltIns::int_struct}),
			getScope(),
			{true, false, true}
			);

	auto array_size_func = new FunctionSymbol(
			"size",
			FunctionTypeInfo(BuiltIns::int_struct, {ref_arr}),
			getScope(),
			{true, false, false}
			);

	getScope()->accept(new FunctionSymbolDefine(array_constructor));
	getScope()->accept(new FunctionSymbolDefine(array_elem_operator));
	getScope()->accept(new FunctionSymbolDefine(array_size_func));

	getScope()->accept(new VariableSymbolDefine(
								new VariableSymbol(
									"~~impl",
									new BuiltInTypeSymbol("~~array_impl", array_size * size_of_type)
									)
				)
			);
}

void AsmArrayNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }

void AsmArrayNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	auto arr = dynamic_cast<StructSymbol*>(getScope());

	CodeGen::emit("jmp _~_"+arr->getName()+"_array_"+arr->getName()+"~ref");
	CodeGen::emit("_"+arr->getName()+"_array_"+arr->getName()+"~ref:");
	CodeGen::emit("ret");
	CodeGen::emit("_~_"+arr->getName()+"_array_"+arr->getName()+"~ref:");


/////////////////////////////////////////////////////////////////////////////
	CodeGen::emit("jmp _~_"+arr->getName()+"_size_"+arr->getName()+"~ref");
	CodeGen::emit("_"+arr->getName()+"_size_"+arr->getName()+"~ref:");

	CodeGen::emit("push rbp");
	CodeGen::emit("mov rbp, rsp");

	CodeGen::emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
	CodeGen::emit("mov rax, [rax]");
	CodeGen::emit("mov qword [rbp - " + std::to_string(array_size * size_of_type + GlobalConfig::int_size) + "], " + std::to_string(array_size));
	CodeGen::emit("lea rax, [rbp - " + std::to_string(array_size * size_of_type + GlobalConfig::int_size) + "]");

	CodeGen::emit("mov rsp, rbp");
	CodeGen::emit("pop rbp");
	CodeGen::emit("ret");

	CodeGen::emit("_~_"+arr->getName()+"_size_"+arr->getName()+"~ref:");
///////////////////////////////////////////////////////////////////////////////


	CodeGen::emit("jmp _~_"+arr->getName()+"_operatorelem_"+arr->getName()+"~ref_int");
	CodeGen::emit("_"+arr->getName()+"_operatorelem_"+arr->getName()+"~ref_int:");
	CodeGen::emit("push rbp");
	CodeGen::emit("mov rbp, rsp");
	CodeGen::emit("mov rbx, [rbp + " + std::to_string(3 * GlobalConfig::int_size) + "]");
	CodeGen::emit("imul rbx, " + std::to_string(size_of_type));

	CodeGen::emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");

	CodeGen::emit("sub rax, rbx");

	CodeGen::emit("mov rsp, rbp");
	CodeGen::emit("pop rbp");
	CodeGen::emit("ret");

	CodeGen::emit("_~_"+arr->getName()+"_operatorelem_"+arr->getName()+"~ref_int:");
}
	
AST* AsmArrayNode::copyTree() const { return new AsmArrayNode(*this); }

std::vector<AST*> AsmArrayNode::getChildren() const { return { }; }
