#include "asmarraynode.hpp"

AsmArrayNode::AsmArrayNode() : array_size(0), size_of_type(0)
{
	setScope(BuiltIns::global_scope);
}

void AsmArrayNode::build_scope() { }

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

		type = static_cast<ClassVariableSymbol*>(static_cast<ReferenceType*>(replace->getType())->getReferredType())->sym;
		ref_type = TypeHelper::getReferenceType(type);
		size_of_type = type->getSize();
	}
	else throw SemanticError("");

	auto arr = dynamic_cast<StructSymbol*>(this->getScope());
	auto ref_arr = TypeHelper::getReferenceType(arr);

	auto array_constructor = new FunctionSymbol(
//			arr->getName(),
			"array",			
			FunctionTypeInfo(ref_arr, {ref_arr}),
			this->getScope(),
			{true, true, false}
			);

	auto array_elem_operator = new FunctionSymbol(
			"operator[]",
			FunctionTypeInfo(ref_type, {ref_arr, BuiltIns::int_struct}),
			this->getScope(),
			{true, false, true}
			);

	auto array_size_func = new FunctionSymbol(
			"size",
			FunctionTypeInfo(BuiltIns::int_struct, {ref_arr}),
			this->getScope(),
			{true, false, false}
			);

	this->getScope()->accept(new FunctionSymbolDefine(array_constructor));
	this->getScope()->accept(new FunctionSymbolDefine(array_elem_operator));
	this->getScope()->accept(new FunctionSymbolDefine(array_size_func));

	this->getScope()->accept(new VariableSymbolDefine(
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
	auto arr = dynamic_cast<StructSymbol*>(this->getScope());

	CodeGen::emit("jmp _~_"+arr->getName()+"_array_"+arr->getName()+"~ref");
	CodeGen::emit("_"+arr->getName()+"_array_"+arr->getName()+"~ref:");
	CodeGen::emit("ret");
	CodeGen::emit("_~_"+arr->getName()+"_array_"+arr->getName()+"~ref:");


/////////////////////////////////////////////////////////////////////////////
	CodeGen::emit("jmp _~_"+arr->getName()+"_size_"+arr->getName()+"~ref");
	CodeGen::emit("_"+arr->getName()+"_size_"+arr->getName()+"~ref:");

	CodeGen::emit("push rbp");
	CodeGen::emit("mov rbp, rsp");

	CodeGen::emit("mov rax, [rbp + 16]");
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
	CodeGen::emit("mov rbx, [rbp + 24]");
	CodeGen::emit("imul rbx, " + std::to_string(size_of_type));

	CodeGen::emit("mov rax, [rbp + 16]");

	CodeGen::emit("sub rax, rbx");

	CodeGen::emit("mov rsp, rbp");
	CodeGen::emit("pop rbp");
	CodeGen::emit("ret");

	CodeGen::emit("_~_"+arr->getName()+"_operatorelem_"+arr->getName()+"~ref_int:");
}
	
AST* AsmArrayNode::copyTree() const { return new AsmArrayNode(*this); }

std::vector<AST*> AsmArrayNode::getChildren() const { return { }; }
