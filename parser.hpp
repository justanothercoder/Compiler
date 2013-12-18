#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "abstractparser.hpp"

#include "structdeclarationnode.hpp"

class Parser : public AbstractParser
{
public:

    Parser(AbstractLexer *lexer); 

    virtual AST* parse();

private:

    string id();

    DeclarationNode* declaration();
    DeclarationNode* structDecl();

    AST* statement();
    
};

#endif
