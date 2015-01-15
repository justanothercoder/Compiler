#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <memory>
#include <fstream>
#include <stack>
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
#include "externnode.hpp"

#include "dotnode.hpp"
#include "callnode.hpp"
#include "variablenode.hpp"
#include "numbernode.hpp"
#include "newexpressionnode.hpp"
#include "bracketnode.hpp"
#include "stringnode.hpp"
#include "addrnode.hpp"
#include "nullnode.hpp"

#include "unsafeblocknode.hpp"
#include "symbol.hpp"
#include "templateparam.hpp"

class Parser : public AbstractParser
{
public:

    Parser(std::unique_ptr<AbstractLexer> lexer);

    virtual std::unique_ptr<AST> parse();

private:
    
    void pushScope();
    void popScope();    
    void rememberSymbol(std::string name, SymbolType type);
    boost::optional<SymbolType> resolveSymbolType(std::string name); 

    std::vector< std::map< std::string, SymbolType > > symbol_table_stack; 
    std::stack<bool> is_in_loop;

    std::string id();
    std::string operator_name();
    TypeInfo typeInfo();
    
    std::vector< std::unique_ptr<ExprNode> > call_params_list();

    TemplateParamsList templateParams();

    std::unique_ptr<DeclarationNode> declaration(boost::optional<std::string> struct_name = boost::none);
    std::unique_ptr<DeclarationNode> structDecl();
    std::unique_ptr<DeclarationNode> variableDecl(boost::optional<std::string> struct_name = boost::none);
    std::unique_ptr<DeclarationNode> functionDecl(boost::optional<std::string> struct_name = boost::none);
    std::unique_ptr<DeclarationNode> varInferDecl(boost::optional<std::string> struct_name = boost::none);

    std::unique_ptr<DeclarationNode> templateDecl(boost::optional<std::string> struct_name, TemplateParamsList template_params); 
    std::unique_ptr<DeclarationNode> templateFunctionDecl(boost::optional<std::string> struct_name, TemplateParamsList template_params);
    std::unique_ptr<DeclarationNode> templateStructDecl(TemplateParamsList template_params);

    std::unique_ptr<AST> break_stat();
    std::unique_ptr<AST> from_import_stat();
    std::unique_ptr<AST> import_stat();
    std::unique_ptr<AST> extern_stat();

    std::unique_ptr<AST> for_stat();
    std::unique_ptr<AST> while_stat();
    std::unique_ptr<AST> if_stat();
    std::unique_ptr<AST> return_stat();
    std::unique_ptr<AST> statement();
    std::unique_ptr<AST> assignment();
    std::unique_ptr<AST> block();

    std::unique_ptr<AST> unsafe_block();

    std::unique_ptr<ExprNode> expression();
    std::unique_ptr<ExprNode> bool_expr();
    std::unique_ptr<ExprNode> relation();
    std::unique_ptr<ExprNode> sum_expr();
    std::unique_ptr<ExprNode> term();
    std::unique_ptr<ExprNode> factor();
    std::unique_ptr<ExprNode> unary_left();
    std::unique_ptr<ExprNode> addr_expr();
    std::unique_ptr<ExprNode> unary_right();
    std::unique_ptr<ExprNode> primary();
    std::unique_ptr<ExprNode> new_expr();
    std::unique_ptr<ExprNode> variable();
    std::unique_ptr<ExprNode> literal();
    std::unique_ptr<ExprNode> number();
    std::unique_ptr<ExprNode> get_string();
    std::unique_ptr<ExprNode> null();

    bool tryAssignment();
    bool tryVarDecl();
    bool tryTypeInfo();

    bool tryModuleName();
};

#endif
