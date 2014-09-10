#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <memory>
#include <fstream>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

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
#include "addrnode.hpp"

#include "unsafeblocknode.hpp"

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
  
    DeclarationNode* declaration(boost::optional<string> struct_name = boost::none);
    DeclarationNode* templateStructDecl();
    DeclarationNode* structDecl();
    DeclarationNode* variableDecl(boost::optional<string> struct_name = boost::none);
    DeclarationNode* functionDecl(boost::optional<string> struct_name = boost::none);
	DeclarationNode* varInferDecl(boost::optional<string> struct_name = boost::none);

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
	ExprNode* addr_expr();
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
