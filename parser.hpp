#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <map>
#include <memory>
#include <stack>
#include <fstream>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include "abstractparser.hpp"
#include "symbol.hpp"
#include "templateparam.hpp"

class DeclarationNode;

enum class SymbolType { FUNCTION, VARIABLE, STRUCT, TEMPLATESTRUCT, TEMPLATEFUNCTION, MODULE };

class Parser : public AbstractParser
{
public:

    Parser(std::unique_ptr<AbstractLexer> lexer);

    virtual ASTNode parse();

private:
    
    void pushScope();
    void popScope();    
    void rememberSymbol(std::string name, SymbolType type);
    boost::optional<SymbolType> resolveSymbolType(std::string name); 

    SymbolType getSymbolType(const Symbol* s);

    std::vector< std::map< std::string, SymbolType > > symbol_table_stack; 
    std::stack<bool> is_in_loop;

    std::string id();
    std::string operator_name();

    TypeInfo typeInfo();
    ParamInfo paramInfo();
    
    std::vector<ASTExprNode> call_params_list();

    TemplateParamInfo templateParamInfo();
    TemplateParamsList templateParams();

    std::unique_ptr<DeclarationNode> declaration(boost::optional<std::string> struct_name = boost::none);
    std::unique_ptr<DeclarationNode> structDecl();
    std::unique_ptr<DeclarationNode> variableDecl(boost::optional<std::string> struct_name = boost::none);
    std::unique_ptr<DeclarationNode> functionDecl(boost::optional<std::string> struct_name = boost::none);
    std::unique_ptr<DeclarationNode> varInferDecl(boost::optional<std::string> struct_name = boost::none);

    std::unique_ptr<DeclarationNode> templateDecl(boost::optional<std::string> struct_name, TemplateParamsList template_params); 
    std::unique_ptr<DeclarationNode> templateFunctionDecl(boost::optional<std::string> struct_name, TemplateParamsList template_params);
    std::unique_ptr<DeclarationNode> templateStructDecl(TemplateParamsList template_params);

    ASTNode break_stat();
    ASTNode from_import_stat();
    ASTNode import_stat();
    ASTNode extern_stat();

    ASTNode for_stat();
    ASTNode while_stat();
    ASTNode if_stat();
    ASTNode return_stat();
    ASTNode statement();
    ASTNode assignment();
    ASTNode block();

    ASTNode unsafe_block();

    ASTExprNode expression();
    ASTExprNode bool_expr();
    ASTExprNode relation();
    ASTExprNode sum_expr();
    ASTExprNode term();
    ASTExprNode factor();
    ASTExprNode unary_left();
    ASTExprNode addr_expr();
    ASTExprNode unary_right();
    ASTExprNode primary();
    ASTExprNode new_expr();
    ASTExprNode variable();
    ASTExprNode literal();
    ASTExprNode number();
    ASTExprNode get_string();
    ASTExprNode null();

    ASTExprNode lambda_expr();

    bool tryAssignment();
    bool tryVarDecl();
    bool tryTypeInfo();

    bool tryModuleName();
};

#endif
