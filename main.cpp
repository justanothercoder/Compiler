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

		BuiltIns::defineBuiltIns();

		root -> scope         = BuiltIns::global_scope;

		root -> build_scope();
		root -> define();
		root -> check ();
	
		CodeObject main_code;
		
		main_code.emit("section .text");


/*
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
*/

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
