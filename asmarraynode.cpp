#include "asmarraynode.hpp"
#include "templateinfo.hpp"
#include "functionsymbol.hpp"

#include "typehelper.hpp"

AsmArrayNode::AsmArrayNode() : size_of_type(0), array_size(0) { scope = BuiltIns::global_scope; }

void AsmArrayNode::define() 
{
	VariableType type, ref_type;

	const auto& template_info = scope -> getTemplateInfo(); 

	if ( template_info.sym -> isIn("size") )
	{
		auto replace = template_info.getReplacement("size");
		
		array_size = boost::get<int>(*replace);
	}
	else throw SemanticError("");

	if ( template_info.sym -> isIn("T") )
	{
		auto replace = template_info.getReplacement("T");

		type = TypeHelper::resolveType(boost::get<std::string>(*replace), scope);
		type.is_ref = false;

		ref_type = type;
		ref_type.is_ref = true;

		size_of_type = type.getSize();
	}
	else throw SemanticError("");

	auto just_int = VariableType(BuiltIns::int_struct);

	auto arr = VariableType(dynamic_cast<StructSymbol*>(scope));

	auto ref_arr = arr;
	ref_arr.is_ref = true;

	auto array_constructor   = new FunctionSymbol("array"     , ref_arr , {ref_arr}          , scope, {true, true, false});
	auto array_elem_operator = new FunctionSymbol("operator[]", ref_type, {ref_arr, just_int}, scope, {true, false, true});
	auto array_size_func     = new FunctionSymbol("size"      , just_int, {ref_arr}          , scope, {true, false, false});

	scope -> accept(new FunctionSymbolDefine(array_constructor));
	scope -> accept(new FunctionSymbolDefine(array_elem_operator));
	scope -> accept(new FunctionSymbolDefine(array_size_func));

	scope -> accept(new VariableSymbolDefine(
								new VariableSymbol(
									"~~impl",
									VariableType(new BuiltInTypeSymbol("~~array_impl", array_size * size_of_type))
									)
				)
			);
}

void AsmArrayNode::check() { }

CodeObject& AsmArrayNode::gen()
{
	auto arr = dynamic_cast<StructSymbol*>(scope);

	code_obj.emit("jmp _~_"+arr -> getName()+"_array_"+arr -> getName()+"~ref");
	code_obj.emit("_"+arr -> getName()+"_array_"+arr -> getName()+"~ref:");
	code_obj.emit("ret");
	code_obj.emit("_~_"+arr -> getName()+"_array_"+arr -> getName()+"~ref:");


/////////////////////////////////////////////////////////////////////////////
	code_obj.emit("jmp _~_"+arr -> getName()+"_size_"+arr -> getName()+"~ref");
	code_obj.emit("_"+arr -> getName()+"_size_"+arr -> getName()+"~ref:");

	code_obj.emit("push rbp");
	code_obj.emit("mov rbp, rsp");

	code_obj.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
	code_obj.emit("mov rax, [rax]");
	code_obj.emit("mov qword [rbp - " + std::to_string(array_size * size_of_type + GlobalConfig::int_size) + "], " + std::to_string(array_size));
	code_obj.emit("lea rax, [rbp - " + std::to_string(array_size * size_of_type + GlobalConfig::int_size) + "]");

	code_obj.emit("mov rsp, rbp");
	code_obj.emit("pop rbp");
	code_obj.emit("ret");

	code_obj.emit("_~_"+arr -> getName()+"_size_"+arr -> getName()+"~ref:");
///////////////////////////////////////////////////////////////////////////////


	code_obj.emit("jmp _~_"+arr -> getName()+"_operatorelem_"+arr -> getName()+"~ref_int");
	code_obj.emit("_"+arr -> getName()+"_operatorelem_"+arr -> getName()+"~ref_int:");
	code_obj.emit("push rbp");
	code_obj.emit("mov rbp, rsp");
	code_obj.emit("mov rbx, [rbp + " + std::to_string(3 * GlobalConfig::int_size) + "]");
	code_obj.emit("imul rbx, " + std::to_string(size_of_type));

	code_obj.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");

	code_obj.emit("sub rax, rbx");

//	code_obj.emit("mov r9, [rbp]");	
//	code_obj.emit("mov [r9 - 8], rax");
//	code_obj.emit("lea rax, [r9 - 8]");

	code_obj.emit("mov rbx, rax");
	code_obj.emit("mov rax, [rbp + 32]");
	code_obj.emit("mov [rax], rbx");

	code_obj.emit("mov rsp, rbp");
	code_obj.emit("pop rbp");
	code_obj.emit("ret");

	code_obj.emit("_~_"+arr -> getName()+"_operatorelem_"+arr -> getName()+"~ref_int:");

	return code_obj;
}
	
AST* AsmArrayNode::copyTree() const { return new AsmArrayNode(*this); }
