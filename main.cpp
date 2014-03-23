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

		BuiltIns::int_struct->accept(new SymbolDefine(BuiltIns::int_type));

		BuiltIns::int_struct->accept(new VariableSymbolDefine(new VariableSymbol("~~impl", VariableType(BuiltIns::int_type), VariableSymbolType::FIELD)));

		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_default_constructor));
		BuiltIns::int_struct->accept(new FunctionSymbolDefine(BuiltIns::int_copy_constructor));

		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::int_assign));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::int_plus));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::int_minus));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::int_mul));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::int_eq));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::int_neq));

		BuiltIns::global_scope->accept(new SymbolDefine(BuiltIns::void_type));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::putchar_func));
		BuiltIns::global_scope->accept(new FunctionSymbolDefine(BuiltIns::getchar_func));

		BuiltIns::array_struct->holder->setScope(BuiltIns::global_scope);
		BuiltIns::global_scope->accept(new SymbolDefine(BuiltIns::array_struct));
		
		GlobalHelper::setDefined(BuiltIns::int_struct);

		root->build_scope();

		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("putchar"));
		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("getchar"));
		GlobalHelper::setDefined(BuiltIns::global_scope->resolve("print"));

		root->define(TemplateInfo());
		root->check (TemplateInfo());
		
		CodeGen::emit("section .text");

		CodeGen::emit("extern _int_operatorassign_int~ref_int");
		CodeGen::emit("extern _int_operatorplus_int~ref_int");
		CodeGen::emit("extern _int_operatorminus_int~ref_int");
		CodeGen::emit("extern _int_operatormul_int~ref_int");	
		CodeGen::emit("extern _int_operatoreq_int~ref_int");
		CodeGen::emit("extern _int_operatorneq_int~ref_int");	
		CodeGen::emit("extern _int_int_int~ref");
		CodeGen::emit("extern _int_int_int~ref_int");
		CodeGen::emit("extern _putchar_int");
		CodeGen::emit("extern _getchar");

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
	catch ( SemanticError& e )
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch ( RecognitionError& e )
	{
		std::cerr << e.what() << '\n';
		return 2;
	}

	return 0;
}
