#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <memory>

#include "abstractparser.hpp"

#include "structdeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"

#include "emptystatementnode.hpp"
#include "binaryoperatornode.hpp"
#include "returnnode.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"

#include "dotnode.hpp"
#include "callnode.hpp"
#include "variablenode.hpp"
#include "numbernode.hpp"
#include "newexpressionnode.hpp"
#include "bracketnode.hpp"

class Parser : public AbstractParser
{
public:

    Parser(AbstractLexer *lexer); 

    virtual AST* parse();

private:

    string id();
	string operator_name();
    TypeInfo typeInfo();
    vector<ExprNode*> call_params_list();
    
    DeclarationNode* declaration(std::shared_ptr<string> struct_name = nullptr);
    DeclarationNode* templateStructDecl();
    DeclarationNode* structDecl();
    DeclarationNode* variableDecl(std::shared_ptr<string> struct_name = nullptr);
    DeclarationNode* functionDecl(std::shared_ptr<string> struct_name = nullptr);

    AST* while_stat();
    AST* if_stat();
    AST* return_stat();
    AST* statement();
    AST* assignment();

    ExprNode* literal();
    ExprNode* expression();
    ExprNode* sum_expr();
    ExprNode* term();
    ExprNode* factor();
    ExprNode* unary_right();    
    ExprNode* primary();
    ExprNode* new_expr();
    ExprNode* variable();
    ExprNode* number();
    
    bool tryAssignment();
    bool tryVarDecl();
};

#endif
