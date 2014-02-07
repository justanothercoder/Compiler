#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "lexer.hpp"
#include "parser.hpp"
#include "builtins.hpp"

using std::shared_ptr;

int main()
{
    std::string filename = "input.txt";
    
    std::ifstream in(filename.c_str());

    if ( !in )
    {
		std::cerr << "No such file '" << filename << "'\n";
		return 0;
    }

    std::string buf = "";

    char c;
    while ( (c = in.get()) != -1 )
		buf += c;    

    try
    {    
		shared_ptr<AbstractLexer> lexer(new Lexer(buf));
		shared_ptr<AbstractParser> parser(new Parser(lexer.get()));
	
		shared_ptr<AST> root(parser->parse());

		root->setScope(BuiltIns::global_scope);

		BuiltIns::global_scope->define(BuiltIns::int_struct);
		
		BuiltIns::int_struct->define(BuiltIns::int_type);

		BuiltIns::int_struct->define(new VariableSymbol("__impl", BuiltIns::int_type, VariableSymbolType::FIELD));
			
		BuiltIns::int_struct->define(BuiltIns::int_constructor);
		BuiltIns::int_struct->define(BuiltIns::int_default_constructor);

		BuiltIns::global_scope->define(BuiltIns::int_assign);
		BuiltIns::global_scope->define(BuiltIns::int_plus);
		BuiltIns::global_scope->define(BuiltIns::int_minus);
		BuiltIns::global_scope->define(BuiltIns::int_mul);

		BuiltIns::global_scope->define(BuiltIns::void_type);
		BuiltIns::global_scope->define(BuiltIns::putchar_func);
		BuiltIns::global_scope->define(BuiltIns::getchar_func);
		
		CodeGen::emit("section .text");

		CodeGen::emit("extern _int_~operatorassign_int~ref_int");
		CodeGen::emit("extern _int_~operatorplus_int_int");
		CodeGen::emit("extern _int_~operatorminus_int_int");
		CodeGen::emit("extern _int_~operatormul_int_int");	
		CodeGen::emit("extern _int_int_int~ref_~~int");
		CodeGen::emit("extern _int_int_int~ref");
		CodeGen::emit("extern _putchar_int");
		CodeGen::emit("extern _getchar");
		
		CodeGen::emit("global _start");
		CodeGen::emit("_start:");

		CodeGen::emit("push rbp");
		CodeGen::emit("mov rbp, rsp");	
		
		root->build_scope();
		root->define();
		root->check();
		root->gen();

		CodeGen::emit("mov rsp, rbp");
		CodeGen::emit("pop rbp");

		CodeGen::emit("mov rax, 60");
		CodeGen::emit("mov rdi, 0");
		CodeGen::emit("syscall");
    }
    catch ( int t )
	{
	//	std::cerr << e.what() << '\n';
    }
    
    in.close();
    
    return 0;
}
