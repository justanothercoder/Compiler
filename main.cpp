#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "lexer.hpp"
#include "parser.hpp"
#include "builtins.hpp"

#include "symboldefine.hpp"
#include "variablesymboldefine.hpp"
#include "functionsymboldefine.hpp"

using std::shared_ptr;

int main()
{
	std::string filename = "input.txt";

	try
	{    
		shared_ptr<AST> root(FileHelper::parse(filename));

		root -> scope         = BuiltIns::global_scope;
		root -> template_info = new TemplateInfo();

		BuiltIns::global_scope -> accept(new SymbolDefine(BuiltIns::int_struct));
		BuiltIns::global_scope -> accept(new SymbolDefine(BuiltIns::ASCII_string));
		BuiltIns::global_scope -> accept(new SymbolDefine(BuiltIns::char_struct));

		BuiltIns::int_struct -> accept(new VariableSymbolDefine(
					new VariableSymbol("~~impl", VariableType(
							new BuiltInTypeSymbol("~~int", GlobalConfig::int_size)
							), VariableSymbolType::FIELD)));

		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_default_constructor));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_copy_constructor));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_assign));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_plus));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_minus));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_mul));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_eq));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_neq));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_div));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_mod));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_and));
		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_or));

		BuiltIns::global_scope -> accept(new SymbolDefine(BuiltIns::void_type));
		BuiltIns::global_scope -> accept(new FunctionSymbolDefine(BuiltIns::putchar_func));
		BuiltIns::global_scope -> accept(new FunctionSymbolDefine(BuiltIns::getchar_func));

		BuiltIns::array_struct -> holder -> scope = BuiltIns::global_scope;
		BuiltIns::global_scope -> accept(new SymbolDefine(BuiltIns::array_struct));
		
		BuiltIns::ASCII_string -> accept(new VariableSymbolDefine(
					new VariableSymbol("~~impl", VariableType(
								new BuiltInTypeSymbol("~~string", 256)
							), VariableSymbolType::FIELD)));

		BuiltIns::ASCII_string -> accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_copy_constructor));
		BuiltIns::ASCII_string -> accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_elem_operator));
		BuiltIns::ASCII_string -> accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_length_func));
		BuiltIns::ASCII_string -> accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_plus_operator));
		BuiltIns::ASCII_string -> accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_assign_operator));
		
		BuiltIns::global_scope -> accept(new FunctionSymbolDefine(BuiltIns::print_ASCII_string_func));

		BuiltIns::global_scope -> accept(new FunctionSymbolDefine(BuiltIns::__fopen_func));
		BuiltIns::global_scope -> accept(new FunctionSymbolDefine(BuiltIns::__fclose_func));
		BuiltIns::global_scope -> accept(new FunctionSymbolDefine(BuiltIns::__fwrite_func));
		BuiltIns::global_scope -> accept(new FunctionSymbolDefine(BuiltIns::__fread_func));
		
	
		BuiltIns::char_struct -> accept(new VariableSymbolDefine(
					new VariableSymbol("~~impl", VariableType(
								new BuiltInTypeSymbol("~~char", GlobalConfig::int_size)
							), VariableSymbolType::FIELD)));

		BuiltIns::char_struct -> accept(new FunctionSymbolDefine(BuiltIns::char_default_constructor));
		BuiltIns::char_struct -> accept(new FunctionSymbolDefine(BuiltIns::char_copy_constructor));
		BuiltIns::char_struct -> accept(new FunctionSymbolDefine(BuiltIns::char_int_constructor));
		BuiltIns::char_struct -> accept(new FunctionSymbolDefine(BuiltIns::char_assign));

		BuiltIns::int_struct -> accept(new FunctionSymbolDefine(BuiltIns::int_char_constructor));

		BuiltIns::int_struct -> is_defined = true;
		BuiltIns::ASCII_string -> is_defined = true;
		BuiltIns::char_struct -> is_defined = true;

		root -> build_scope();

		BuiltIns::global_scope -> resolve("putchar") -> is_defined = true;
		BuiltIns::global_scope -> resolve("getchar") -> is_defined = true;
		BuiltIns::global_scope -> resolve("print") -> is_defined   = true;

		BuiltIns::global_scope -> resolve("__fopen") -> is_defined  = true;
		BuiltIns::global_scope -> resolve("__fclose") -> is_defined = true;
		BuiltIns::global_scope -> resolve("__fwrite") -> is_defined = true;
		BuiltIns::global_scope -> resolve("__fread") -> is_defined  = true;
		
		root -> define();
		root -> check ();
	
		CodeObject main_code;
		
		main_code.emit("section .text");

		main_code.emit("extern " + BuiltIns::int_assign -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::int_plus -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::int_minus -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::int_mul -> getScopedTypedName());	
		main_code.emit("extern " + BuiltIns::int_eq -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::int_neq -> getScopedTypedName());	
		main_code.emit("extern " + BuiltIns::int_div -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::int_mod -> getScopedTypedName());	
		main_code.emit("extern " + BuiltIns::int_and -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::int_or -> getScopedTypedName());	
		main_code.emit("extern " + BuiltIns::int_default_constructor -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::int_copy_constructor -> getScopedTypedName());

		main_code.emit("extern " + BuiltIns::putchar_func -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::getchar_func -> getScopedTypedName());
		
		main_code.emit("extern " + BuiltIns::ASCII_string_copy_constructor -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::ASCII_string_elem_operator -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::ASCII_string_length_func -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::ASCII_string_plus_operator -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::ASCII_string_assign_operator -> getScopedTypedName());
		
		main_code.emit("extern " + BuiltIns::print_ASCII_string_func -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::__fopen_func -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::__fclose_func -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::__fwrite_func -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::__fread_func -> getScopedTypedName());
		
		main_code.emit("extern " + BuiltIns::char_assign -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::char_copy_constructor -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::char_default_constructor -> getScopedTypedName());
		main_code.emit("extern " + BuiltIns::char_int_constructor -> getScopedTypedName());
		
		main_code.emit("extern " + BuiltIns::int_char_constructor -> getScopedTypedName());

		main_code.emit("global _start");
		main_code.emit("_start:");

		main_code.emit("push rbp");
		main_code.emit("mov rbp, rsp");	

		if ( root -> scope -> getVarAlloc().getSpace() > 0 )		
			main_code.emit("sub rsp, " + std::to_string(root -> scope -> getVarAlloc().getSpace()));
		main_code.emit("sub rsp, " + std::to_string(root -> scope -> getTempAlloc().getSpaceNeeded()));

		main_code.emit(root -> gen().getCode());

		main_code.emit("mov rsp, rbp");
		main_code.emit("pop rbp");

		main_code.emit("mov rax, 60");
		main_code.emit("mov rdi, 0");
		main_code.emit("syscall");

		main_code.gen();
	}
//	catch ( SemanticError& e )
//	{
//		std::cerr << e.what() << '\n';
//		return 1;
//	}
	catch ( RecognitionError& e )
	{
		std::cerr << e.what() << '\n';
		return 2;
	}

	return 0;
}
