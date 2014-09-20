#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "lexer.hpp"
#include "parser.hpp"
#include "builtins.hpp"
#include "filehelper.hpp"
#include "functionsymbol.hpp"
#include "globalhelper.hpp"

#include "genssavisitor.hpp"

using std::shared_ptr;

int main()
{
	std::string filename = "input.txt";

	try
	{    
		shared_ptr<AST> root(FileHelper::parse(filename));

		BuiltIns::defineBuiltIns();

		root -> scope = BuiltIns::global_scope;

		root -> build_scope();
		root -> define();
		root -> check ();

		GenSSAVisitor visitor;
		root -> accept(&visitor);
		std::cerr << visitor.toString() << '\n';

		CodeObject main_code;
		
		main_code.emit("section .text");

		for ( auto p : GlobalHelper::has_definition )
		{
			if ( !p.second )
				main_code.emit("extern " + p.first -> getScopedTypedName());
		}

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

//		main_code.gen();
	}
//	catch ( SemanticError& e )
//	{
//		std::cerr << e.what() << '\n';
//		return 1;
//	}
//	catch ( RecognitionError& e )
//	{
//		std::cerr << e.what() << '\n';
//		return 2;
//	}
	catch ( int x )
	{
		return 1;
	}

	return 0;
}
