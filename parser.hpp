#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <memory>
#include <fstream>

#include "abstractparser.hpp"

#include "structdeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"

#include "varinfertypedeclarationnode.hpp"

#include "statementnode.hpp"
#include "binaryoperatornode.hpp"
#include "unarynode.hpp"
#include "returnnode.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "importnode.hpp"

#include "dotnode.hpp"
#include "callnode.hpp"
#include "variablenode.hpp"
#include "numbernode.hpp"
#include "newexpressionnode.hpp"
#include "bracketnode.hpp"
#include "stringnode.hpp"

#include "unsafeblocknode.hpp"

#include "optional.hpp"

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
  
    DeclarationNode* declaration(optional<string> struct_name = optional<string>::empty());
    DeclarationNode* templateStructDecl();
    DeclarationNode* structDecl();
    DeclarationNode* variableDecl(optional<string> struct_name = optional<string>::empty());
    DeclarationNode* functionDecl(optional<string> struct_name = optional<string>::empty());
	DeclarationNode* varInferDecl(optional<string> struct_name = optional<string>::empty());

	AST* import_stat();
	
	AST* for_stat();
    AST* while_stat();
    AST* if_stat();
    AST* return_stat();
    AST* statement();
    AST* assignment();
	AST* block();

	AST* unsafe_block();

    ExprNode* expression();
	ExprNode* bool_expr();
	ExprNode* relation();
    ExprNode* sum_expr();
    ExprNode* term();
    ExprNode* factor();
	ExprNode* unary_left();
    ExprNode* unary_right();    
    ExprNode* primary();
    ExprNode* new_expr();
    ExprNode* variable();
    ExprNode* literal();
    ExprNode* number();
   	ExprNode* get_string();

    bool tryAssignment();
    bool tryVarDecl();
    bool tryTypeInfo();
};

#endif
