#include <iostream>
#include <fstream>
#include <string>

#include "lexer.hpp"
#include "parser.hpp"
#include "globalscope.hpp"
#include "builtintypesymbol.hpp"

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

	AST *root = parser->parse();
	root->scope = new GlobalScope();

	root->scope->define(new BuiltInTypeSymbol("int", sizeof(int*)));
	
	root->process();

    }
    catch ( RecognitionError& e )
    {
	std::cerr << e.what() << '\n';
    }
    
    in.close();
    
    return 0;
}
