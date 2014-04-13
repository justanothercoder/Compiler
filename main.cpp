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

		root->setScope(BuiltIns::global_scope);

		BuiltIns::global_scope->accept(new SymbolDefine(BuiltIns::int_struct));
		BuiltIns::global_scope->accept(new SymbolDefine(BuiltIns::ASCII_string));

		BuiltIns::int_struct->accept(new VariableSymbolDefine(
					new VariableSymbol("~~impl", VariableType(
							new BuiltInTypeSymbol("~~int", GlobalConfig::int_size)
							), VariableSymbolType::FIELD)));

		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_default_constructor));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_copy_constructor));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_assign));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_plus));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_minus));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_mul));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_eq));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_neq));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_div));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_mod));

		BuiltIns::global_scope->accept(new SymbolDefine(BuiltIns::void_type));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::putchar_func));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::getchar_func));

		BuiltIns::array_struct->holder->setScope(BuiltIns::global_scope);
		BuiltIns::global_scope->accept(new SymbolDefine(BuiltIns::array_struct));
		
		BuiltIns::ASCII_string->accept(new VariableSymbolDefine(
					new VariableSymbol("~~impl", VariableType(
								new BuiltInTypeSymbol("~~string", 256)
							), VariableSymbolType::FIELD)));

		BuiltIns::ASCII_string->accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_copy_constructor));
		BuiltIns::ASCII_string->accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_elem_operator));
		BuiltIns::ASCII_string->accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_length_func));
		BuiltIns::ASCII_string->accept(new FunctionSymbolDefine(BuiltIns::ASCII_string_plus_operator));
		
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::print_ASCII_string_func));

		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::__fopen_func));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::__fclose_func));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::__fwrite_func));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::__fread_func));
		
		GlobalHelper::setDefined(BuiltIns::int_struct);
		GlobalHelper::setDefined(BuiltIns::ASCII_string);

		root->build_scope();

		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("putchar"));
		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("getchar"));
		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("print"));

		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("__fopen"));
		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("__fclose"));
		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("__fwrite"));
		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("__fread"));
		
		root->define(TemplateInfo());
		root->check (TemplateInfo());
		
		CodeGen::emit("section .text");

		CodeGen::emit("extern " + BuiltIns::int_assign->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::int_plus->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::int_minus->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::int_mul->getScopedTypedName());	
		CodeGen::emit("extern " + BuiltIns::int_eq->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::int_neq->getScopedTypedName());	
		CodeGen::emit("extern " + BuiltIns::int_div->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::int_mod->getScopedTypedName());	
		CodeGen::emit("extern " + BuiltIns::int_default_constructor->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::int_copy_constructor->getScopedTypedName());

		CodeGen::emit("extern " + BuiltIns::putchar_func->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::getchar_func->getScopedTypedName());
		
		CodeGen::emit("extern " + BuiltIns::ASCII_string_copy_constructor->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::ASCII_string_elem_operator->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::ASCII_string_length_func->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::ASCII_string_plus_operator->getScopedTypedName());
		
		CodeGen::emit("extern " + BuiltIns::print_ASCII_string_func->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::__fopen_func->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::__fclose_func->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::__fwrite_func->getScopedTypedName());
		CodeGen::emit("extern " + BuiltIns::__fread_func->getScopedTypedName());

		CodeGen::emit("global _start");
		CodeGen::emit("_start:");

		CodeGen::emit("push rbp");
		CodeGen::emit("mov rbp, rsp");	

		if ( root->getScope()->get_valloc()->getSpace() > 0 )		
			CodeGen::emit("sub rsp, " + std::to_string(root->getScope()->get_valloc()->getSpace()));

		root->gen(TemplateInfo());

		CodeGen::emit("mov rsp, rbp");
		CodeGen::emit("pop rbp");

		CodeGen::emit("mov rax, 60");
		CodeGen::emit("mov rdi, 0");
		CodeGen::emit("syscall");
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
