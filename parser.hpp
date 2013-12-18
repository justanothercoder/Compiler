#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "abstractparser.hpp"

class Parser : public AbstractParser
{
public:

    Parser(AbstractLexer *lexer); 

    virtual AST* parse();

private:

    string id();

    AST* declaration();
    AST* structDecl();

    AST* statement();
    
};

#endif
