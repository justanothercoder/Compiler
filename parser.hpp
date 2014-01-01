#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "abstractparser.hpp"

#include "structdeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "functiondeclarationnode.hpp"

class Parser : public AbstractParser
{
public:

    Parser(AbstractLexer *lexer); 

    virtual AST* parse();

private:

    string id();

    DeclarationNode* declaration();
    DeclarationNode* structDecl();
    DeclarationNode* variableDecl();
    DeclarationNode* functionDecl();
    
    AST* statement();
    
};

#endif
