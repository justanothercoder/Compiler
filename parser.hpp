#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "abstractparser.hpp"

#include "structdeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "functiondeclarationnode.hpp"

#include "emptystatementnode.hpp"
#include "assignmentnode.hpp"
#include "returnnode.hpp"

#include "callnode.hpp"
#include "variablenode.hpp"
#include "numbernode.hpp"

class Parser : public AbstractParser
{
public:

    Parser(AbstractLexer *lexer); 

    virtual AST* parse();

private:

    string id();
    TypeInfo type_info();
    pair<string, TypeInfo > var_and_type();
    
    DeclarationNode* declaration();
    DeclarationNode* structDecl();
    DeclarationNode* variableDecl();
    DeclarationNode* functionDecl();

    AST* return_stat();
    AST* statement();
    AST* assignment();

    ExprNode* literal();
    ExprNode* expression();
    ExprNode* factor();
    ExprNode* unary_right();
    ExprNode* primary();
    ExprNode* variable();
    ExprNode* number();

    bool tryAssignment();
};

#endif
