#include <iostream>
#include <fstream>
#include <string>

#include "lexer.hpp"
#include "parser.hpp"
#include "globalscope.hpp"
#include "builtintypesymbol.hpp"
#include "typehelper.hpp"

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
	AbstractLexer *lexer = new Lexer(buf);
	AbstractParser *parser = new Parser(lexer);
	
	AST* root = parser->parse();

	GlobalHelper::setASTScope(root, new GlobalScope());

	BuiltInTypeSymbol *int_type = new BuiltInTypeSymbol("int", sizeof(int*));	

	auto int_ref = TypeHelper::getReferenceType(int_type);
	
	root->getScope()->define(int_type);
	root->getScope()->define(new FunctionSymbol("operator=", FunctionTypeInfo(int_ref, {int_ref, int_type}), root->getScope(), true));
	root->getScope()->define(new FunctionSymbol("operator+", FunctionTypeInfo(int_type, {int_type, int_type}), root->getScope(), true));
	root->getScope()->define(new FunctionSymbol("operator-", FunctionTypeInfo(int_type, {int_type, int_type}), root->getScope(), true));
	root->getScope()->define(new FunctionSymbol("operator*", FunctionTypeInfo(int_type, {int_type, int_type}), root->getScope(), true));

	CodeGen::emit("section .text");

	CodeGen::emit("extern _~operator_assign_int~ref_int");
	CodeGen::emit("extern _~operator_plus_int_int");
	CodeGen::emit("extern _~operator_minus_int_int");
	CodeGen::emit("extern _~operator_mul_int_int");	

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
