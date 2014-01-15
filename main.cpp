#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "lexer.hpp"
#include "parser.hpp"
#include "builtins.hpp"

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
	std::shared_ptr<AbstractLexer> lexer(new Lexer(buf));
	std::shared_ptr<AbstractParser> parser(new Parser(lexer.get()));
	
	std::shared_ptr<AST> root(parser->parse());

	root->setScope(BuiltIns::global_scope);

	root->getScope()->define(BuiltIns::int_type);
	root->getScope()->define(BuiltIns::int_assign);
	root->getScope()->define(BuiltIns::int_plus);
	root->getScope()->define(BuiltIns::int_minus);
	root->getScope()->define(BuiltIns::int_mul);

	CodeGen::emit("section .text");

	CodeGen::emit("extern _~operatorassign_int~ref_int");
	CodeGen::emit("extern _~operatorplus_int_int");
	CodeGen::emit("extern _~operatorminus_int_int");
	CodeGen::emit("extern _~operatormul_int_int");	

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
