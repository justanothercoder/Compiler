#include "filehelper.hpp"
#include "filenotfoundexception.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"

AST* FileHelper::parse(std::string filename)
{
    return std::shared_ptr<AbstractParser>(new Parser(
            std::shared_ptr<AbstractLexer>(new Lexer(
                    extractContents(filename)
                                           )
                                          ).get()
                                           )
                                          )->parse();
}

string FileHelper::extractContents(std::string filename)
{
    std::ifstream in(filename.c_str());

    if ( !in )
        throw FileNotFoundException("No such file '" + filename + "'\n");

    std::string buf = "";

    char c;
    while ( (c = in.get()) != -1 )
        buf += c;

    in.close();

    return buf;
}
