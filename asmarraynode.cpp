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

void AsmArrayNode::define() 
{
	const Type *type;
    const Type *ref_type;

	const auto& template_info = scope -> getTemplateInfo(); 

	if ( template_info.sym -> isIn("size") )
	{
		auto replace = template_info.getReplacement("size");
		
		array_size = boost::get<int>(*replace);
	}
	else throw SemanticError("");

	auto arr = dynamic_cast<StructSymbol*>(scope);
	arr -> is_unsafe = true;

	if ( template_info.sym -> isIn("T") )
	{
		auto replace = template_info.getReplacement("T");
		type         = scope -> fromTypeInfo(boost::get<TypeInfo>(*replace));
		ref_type     = TypeFactory::getReference(type);
		size_of_type = type -> getSize();
	}
	else throw SemanticError("");

	auto just_int = BuiltIns::int_type;

	auto ref_arr = TypeFactory::getReference(arr);

	array_constructor   = new FunctionSymbol("array"     , ref_arr , {ref_arr}          , scope, {true, true, false});
	array_elem_operator = new FunctionSymbol("operator[]", ref_type, {ref_arr, just_int}, scope, {true, false, true});
	array_size_func     = new FunctionSymbol("size"      , just_int, {ref_arr}          , scope, {true, false, false});

	GlobalHelper::has_definition[array_constructor]   = true;
	GlobalHelper::has_definition[array_elem_operator] = true;
	GlobalHelper::has_definition[array_size_func]     = true;

	array_constructor   -> is_unsafe = true;
	array_elem_operator -> is_unsafe = true;
	array_size_func     -> is_unsafe = true;

	scope -> define(array_constructor);
	scope -> define(array_elem_operator);
	scope -> define(array_size_func);

	scope -> define(new VariableSymbol( "~~impl", new BuiltInTypeSymbol("~~array_impl", array_size * size_of_type)));
}

void AsmArrayNode::check() { }

CodeObject& AsmArrayNode::gen()
{
	auto arr = dynamic_cast<StructSymbol*>(scope);

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

void AsmArrayNode::accept(ASTVisitor *visitor)
{
	visitor -> visit(this);
}
