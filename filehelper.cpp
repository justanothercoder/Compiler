#include "filehelper.hpp"

AST* FileHelper::parse(string filename) 
{ 
	return shared_ptr<AbstractParser>(new Parser(
				shared_ptr<AbstractLexer>(new Lexer(
						extractContents(filename)
						)
					).get()
				)
			)->parse(); 
}
	
string FileHelper::extractContents(string filename)
{
	std::ifstream in(filename.c_str());

	if ( !in )
		throw exception();
//		throw FileNotFoundException("No such file '" + filename + "'\n");

	std::string buf = "";

	char c;
	while ( (c = in.get()) != -1 )
		buf += c;    

	in.close();

	return buf;
}
