#include "parser.hpp"
#include "semanticerror.hpp"
#include "breaknode.hpp"
#include "functionnode.hpp"
#include "modulenode.hpp"
#include "filehelper.hpp"
#include "modulememberaccessnode.hpp"
#include "compilableunit.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "templatefunctionnode.hpp"
#include "structdeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "statementnode.hpp"
#include "binaryoperatornode.hpp"
#include "unarynode.hpp"
#include "unsafeblocknode.hpp"
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
#include "comp.hpp"

#include "logger.hpp"

Parser::Parser(std::unique_ptr<AbstractLexer> lexer) : AbstractParser(std::move(lexer))
{

}
    
void Parser::pushScope() { symbol_table_stack.emplace_back(); }
void Parser::popScope()  { symbol_table_stack.pop_back(); }

SymbolType_ Parser::getSymbolType(const Symbol* s)
{
    if      ( s -> isVariable() ) { return SymbolType_::VARIABLE; }
    else if ( s -> isFunction() ) { return SymbolType_::FUNCTION; }
    else if ( s -> isType()     ) { return SymbolType_::STRUCT; }
    else if ( s -> isModule()   ) { return SymbolType_::MODULE; }
    else { throw; }
}

void Parser::rememberSymbol(std::string name, SymbolType_ type)
{
    if ( isSpeculating() )
        return;

    auto& table = symbol_table_stack.back();

    auto it = table.find(name);

    if ( it == std::end(table) )
        table[name] = type;
    else
    {
        if ( !(it -> second == SymbolType_::FUNCTION && type == SymbolType_::FUNCTION) )
            throw SemanticError("Symbol " + name + " is already defined");
    }
}
    
boost::optional<SymbolType_> Parser::resolveSymbolType(std::string name)
{
    boost::optional<SymbolType_> sym_type = boost::none;
    for ( auto it = symbol_table_stack.rbegin(); it != symbol_table_stack.rend(); ++it )
    {
        auto map_it = it -> find(name);

        if ( map_it != std::end(*it) )
        {
            sym_type = map_it -> second;
            break;
        }            
    }
    return sym_type;
}

std::vector<ParamInfo> Parser::formalParams()
{
    auto params = std::vector<ParamInfo>{ };
    match(TokenType::LPAREN);

    if ( getTokenType(1) != TokenType::RPAREN )
    {
        params.emplace_back(paramInfo());
        while ( getTokenType(1) != TokenType::RPAREN )
        {
            match(TokenType::COMMA);
            params.emplace_back(paramInfo());
        }
    }

    match(TokenType::RPAREN);
    return params;
}

ASTNode Parser::parse()
{
    pushScope();
    
    is_in_loop.push(false);

    rememberSymbol("int", SymbolType_::STRUCT);
    rememberSymbol("void", SymbolType_::STRUCT);
    rememberSymbol("char", SymbolType_::STRUCT);
    rememberSymbol("class", SymbolType_::STRUCT);
    rememberSymbol("putchar", SymbolType_::FUNCTION);
    rememberSymbol("print", SymbolType_::FUNCTION);
    rememberSymbol("string", SymbolType_::STRUCT);

    auto statements = std::vector<ASTNode>{ };

    while ( getTokenType(1) != TokenType::EOF_TYPE )
        statements.push_back(statement());

    popScope();

    return std::make_unique<StatementNode>(std::move(statements));
}

ASTNode Parser::statement()
{
    if ( getTokenType(1) == TokenType::SEMICOLON )
    {
        match(TokenType::SEMICOLON);
        return std::make_unique<StatementNode>(std::vector<ASTNode>{ });
    }
    else if ( getTokenType(1) == TokenType::STRUCT
              || getTokenType(1) == TokenType::DEF
              || getTokenType(1) == TokenType::TEMPLATE
              || getTokenType(1) == TokenType::VAR
              || tryVarDecl()
            )
        return declaration();
    else if ( getTokenType(1) == TokenType::RETURN ) return return_stat();
    else if ( getTokenType(1) == TokenType::IF     ) return if_stat();
    else if ( getTokenType(1) == TokenType::WHILE  ) return while_stat();
    else if ( getTokenType(1) == TokenType::FOR    ) return for_stat();
    else if ( getTokenType(1) == TokenType::LBRACE ) return block();
    else if ( getTokenType(1) == TokenType::IMPORT ) return import_stat();
    else if ( getTokenType(1) == TokenType::UNSAFE ) return unsafe_block();
    else if ( getTokenType(1) == TokenType::EXTERN ) return extern_stat();
    else if ( getTokenType(1) == TokenType::FROM   ) return from_import_stat();
    else if ( getTokenType(1) == TokenType::BREAK  ) return break_stat();
    else if ( tryAssignment() )                      return assignment();
    else                                             return expression();
}

ASTNode Parser::block()
{
    match(TokenType::LBRACE);

    auto statements = std::vector< ASTNode >{ };

    while ( getTokenType(1) != TokenType::RBRACE )
        statements.push_back(statement());

    match(TokenType::RBRACE);

    return std::make_unique<StatementNode>(std::move(statements));
}

std::unique_ptr<DeclarationNode> Parser::declaration(boost::optional<std::string> struct_name)
{
    if      ( getTokenType(1) == TokenType::STRUCT )   return structDecl();
    else if ( getTokenType(1) == TokenType::DEF )      return functionDecl(struct_name);
    else if ( getTokenType(1) == TokenType::VAR )      return varInferDecl(struct_name);
    else if ( getTokenType(1) == TokenType::TEMPLATE ) return templateDecl(struct_name, templateParamsInfo()); 
    else if ( tryVarDecl() )                           return variableDecl(struct_name);
    else                                               throw RecognitionError("Declaration expected", getToken(1).line, getToken(1).symbol);
}

bool Parser::tryVarDecl()
{
    bool success = true;

    mark();

    try
    {
        typeInfo();
        id();
    }
    catch ( RecognitionError& re )
    {
        success = false;
    }

    release();

    return success;
}

std::unique_ptr<DeclarationNode> Parser::variableDecl(boost::optional<std::string> struct_name)
{
    auto type_info = typeInfo();
    auto var_name  = id();

    auto constructor_params = std::vector<ASTExprNode>{ };

    if ( getTokenType(1) == TokenType::LPAREN )
        constructor_params = call_params_list();
    else if ( getTokenType(1) == TokenType::ASSIGN )
    {
        match(TokenType::ASSIGN);
        constructor_params.push_back(expression());
    }

    rememberSymbol(var_name, SymbolType_::VARIABLE);

    return std::make_unique<VariableDeclarationNode>(std::move(var_name)
                                                   , std::move(type_info)
                                                   , bool(struct_name)
                                                   , std::move(constructor_params));
}

std::unique_ptr<DeclarationNode> Parser::structDecl()
{
    match(TokenType::STRUCT);

    auto struct_name = id();

    auto struct_in = std::vector<ASTNode>{ };
    match(TokenType::LBRACE);

    auto functions = std::vector<ASTNode>{ };

    rememberSymbol(struct_name, SymbolType_::STRUCT);
    pushScope();
    rememberSymbol("this", SymbolType_::VARIABLE);

    while ( getTokenType(1) != TokenType::RBRACE )
    {
        while ( getTokenType(1) == TokenType::SEMICOLON )
            match(TokenType::SEMICOLON);

        if ( getTokenType(1) != TokenType::RBRACE )
        {
            if ( getTokenType(1) == TokenType::DEF )
                functions.push_back(functionDecl(struct_name));
            else
                struct_in.push_back(declaration(struct_name));
        }
    }

    match(TokenType::RBRACE);
    
    std::move(std::begin(functions), std::end(functions), std::back_inserter(struct_in));
    popScope();

    return std::make_unique<StructDeclarationNode>(std::move(struct_name), std::move(struct_in));
}

std::unique_ptr<DeclarationNode> Parser::templateDecl(boost::optional<std::string> struct_name, TemplateParamsInfo template_params_info)
{
    if ( getTokenType(1) == TokenType::DEF )
        return templateFunctionDecl(struct_name, template_params_info);
    else
        return templateStructDecl(template_params_info);
}

TemplateParamsInfo Parser::templateParamsInfo()
{
    match(TokenType::TEMPLATE);
    match(TokenType::LESS);

    auto template_params_info = TemplateParamsInfo{ };

    if ( getTokenType(1) != TokenType::GREATER )
    {
        template_params_info.emplace_back(paramInfo());
        while ( getTokenType(1) == TokenType::COMMA )
        {
            match(TokenType::COMMA);
            template_params_info.emplace_back(paramInfo());
        }
    }

    match(TokenType::GREATER);
    return template_params_info;
}
    
std::unique_ptr<DeclarationNode> Parser::templateStructDecl(TemplateParamsInfo template_params_info)
{
    match(TokenType::STRUCT);

    auto struct_name = id();

    if ( getTokenType(1) == TokenType::LESS )
    {
        match(TokenType::LESS);

        match(TokenType::GREATER);
    }

    auto struct_in = std::vector<ASTNode>{ };
    match(TokenType::LBRACE);

    rememberSymbol(struct_name, SymbolType_::TEMPLATESTRUCT);
    pushScope();

    for ( const auto& param_info : template_params_info )
    {
        if ( param_info.second.name() == "class" )
            rememberSymbol(param_info.first, SymbolType_::STRUCT);
        else
            rememberSymbol(param_info.first, SymbolType_::VARIABLE);            
    }

    while ( getTokenType(1) != TokenType::RBRACE )
    {
        while ( getTokenType(1) == TokenType::SEMICOLON )
            match(TokenType::SEMICOLON);

        if ( getTokenType(1) != TokenType::RBRACE )
            struct_in.push_back(declaration(struct_name));
    }

    match(TokenType::RBRACE);
    popScope();

    return std::make_unique<TemplateStructDeclarationNode>(std::move(struct_name), std::move(struct_in), std::move(template_params_info));
}

std::unique_ptr<DeclarationNode> Parser::templateFunctionDecl(boost::optional<std::string> struct_name, TemplateParamsInfo template_params_info)
{
    match(TokenType::DEF);

    bool is_unsafe = false;

    if ( getTokenType(1) == TokenType::UNSAFE )
    {
        is_unsafe = true;
        match(TokenType::UNSAFE);
    }

    bool is_operator = getTokenType(1) == TokenType::OPERATOR;

    auto function_name = (is_operator ? operator_name() : id());
    
    rememberSymbol(function_name, SymbolType_::TEMPLATEFUNCTION);

    pushScope();
    
    for ( const auto& param_info : template_params_info )
        rememberSymbol(param_info.first, (param_info.second.name() == "class" ? SymbolType_::STRUCT : SymbolType_::VARIABLE));

    auto params = formalParams();
    for ( const auto& param : params )
        rememberSymbol(param.first, SymbolType_::VARIABLE);

    TypeInfo return_type;

    bool is_constructor = (bool(struct_name) && function_name == *struct_name);

    if ( !is_constructor )
    {
        if ( getTokenType(1) == TokenType::COLON )
        {
            match(TokenType::COLON);
            return_type = typeInfo();
        }
        else
            return_type = TypeInfo("void", false, false);
    }
    else
        return_type = TypeInfo(*struct_name, true, false);

    auto statements = block();

    popScope();

    return std::make_unique<TemplateFunctionDeclarationNode>(std::move(function_name)
                                                           , std::move(FunctionDeclarationInfo(return_type, params))
                                                           , std::move(statements)
                                                           , FunctionTraits{bool(struct_name), is_constructor, is_operator}
                                                           , is_unsafe
                                                           , std::move(template_params_info)
                                                          );

}

std::unique_ptr<DeclarationNode> Parser::functionDecl(boost::optional<std::string> struct_name)
{
    match(TokenType::DEF);

    bool is_unsafe = false;

    if ( getTokenType(1) == TokenType::UNSAFE )
    {
        is_unsafe = true;
        match(TokenType::UNSAFE);
    }

    bool is_operator = getTokenType(1) == TokenType::OPERATOR;

    auto function_name = (is_operator ? operator_name() : id());
    
    rememberSymbol(function_name, SymbolType_::FUNCTION);
    pushScope();
    
    auto params = formalParams();
    for ( const auto& param : params )
        rememberSymbol(param.first, SymbolType_::VARIABLE);

    TypeInfo return_type;

    bool is_constructor = (bool(struct_name) && function_name == *struct_name);

    if ( !is_constructor )
    {
        if ( getTokenType(1) == TokenType::COLON )
        {
            match(TokenType::COLON);
            return_type = typeInfo();
        }
        else
        {
            return_type = TypeInfo("void", false, false);
        }
    }
    else
    {
        return_type = TypeInfo(*struct_name, true, false);
    }

    auto statements = block();

    popScope();

    return std::make_unique<FunctionDeclarationNode>(std::move(function_name)
                                                   , std::move(FunctionDeclarationInfo(return_type, params))
                                                   , std::move(statements)
                                                   , FunctionTraits{bool(struct_name), is_constructor, is_operator}
                                                   , is_unsafe
                                                  );
}

std::string Parser::id()
{
    auto name = getToken(1).text;
    match(TokenType::ID);
    return std::move(name);
}

std::string Parser::operator_name()
{
    match(TokenType::OPERATOR);

    switch ( getTokenType(1) )
    {
    case TokenType::PLUS : match(TokenType::PLUS); return "operator+";
    case TokenType::MINUS: match(TokenType::MINUS); return "operator-";
    case TokenType::MUL  : match(TokenType::MUL); return "operator*";
    case TokenType::DIV  : match(TokenType::DIV); return "operator/";
    case TokenType::MOD  : match(TokenType::MOD); return "operator%";
    case TokenType::LPAREN:
    {
        match(TokenType::LPAREN);
        match(TokenType::RPAREN);
        return "operator()";
    }
    case TokenType::LBRACKET:
    {
        match(TokenType::LBRACKET);
        match(TokenType::RBRACKET);
        return "operator[]";
    }
    case TokenType::ID: return "operator " + id();
    default: throw SemanticError("No such operator " + getToken(1).text);
    }
}

ASTExprNode Parser::literal()
{
    return (getTokenType(1) == TokenType::STRING ? get_string() : number());
}

ASTExprNode Parser::variable()
{
    auto var_name = id();

    auto sym_type = resolveSymbolType(var_name);

    if ( !sym_type )
        throw SemanticError("No such symbol " + var_name);

    switch ( *sym_type )
    {
        case SymbolType_::VARIABLE: return std::make_unique<VariableNode>(var_name);
        case SymbolType_::MODULE  : return std::make_unique<ModuleNode>(var_name);
        case SymbolType_::FUNCTION: return std::make_unique<FunctionNode>(var_name);
        case SymbolType_::TEMPLATEFUNCTION:
        {
            auto template_arguments = TemplateArgumentsInfo{ };
            
            if ( getTokenType(1) == TokenType::LESS )
            {
                match(TokenType::LESS);

                if ( getTokenType(1) != TokenType::GREATER )
                {
                    template_arguments.emplace_back(templateArgumentInfo());
                    while ( getTokenType(1) == TokenType::COMMA )
                    {
                        match(TokenType::COMMA);
                        template_arguments.emplace_back(templateArgumentInfo());
                    }
                }

                match(TokenType::GREATER);
            }

            return std::make_unique<TemplateFunctionNode>(std::move(var_name), std::move(template_arguments));
        }
        default: throw std::logic_error("Internal error");
    }
}

ASTExprNode Parser::get_string()
{
    auto str = getToken(1).text;
    match(TokenType::STRING);
    return std::make_unique<StringNode>(str);
}

ASTExprNode Parser::number()
{
    auto num = getToken(1).text;
    match(TokenType::NUMBER);
    return std::make_unique<NumberNode>(num);
}

ASTExprNode Parser::null()
{
    match(TokenType::NULLTOKEN);
    return std::make_unique<NullNode>();
}

ASTExprNode Parser::primary()
{
    switch ( getTokenType(1) )
    {
        case TokenType::NUMBER: case TokenType::STRING: return literal();
        case TokenType::NEW:                            return new_expr();
        case TokenType::NULLTOKEN:                      return null();
        case TokenType::LBRACKET:                       return lambda_expr();
        case TokenType::LPAREN:
        {
            match(TokenType::LPAREN);
            auto expr = expression();
            match(TokenType::RPAREN);
            return expr;
        }
        default: return variable();
    }
}

ASTExprNode Parser::lambda_expr()
{
    match(TokenType::LBRACKET);
    // TODO add capture
    match(TokenType::RBRACKET);

    auto params = formalParams();
    return nullptr;
}

ASTExprNode Parser::unary_right()
{
    ASTExprNode res;

    if ( tryModuleName() )
    {
        auto module_name = id();
        match(TokenType::DOT);
        auto member = id();
        res = std::make_unique<ModuleMemberAccessNode>(module_name, member);
    }
    else
    {
        res = primary();
    }

    while ( getTokenType(1) == TokenType::LPAREN || getTokenType(1) == TokenType::DOT || getTokenType(1) == TokenType::LBRACKET )
    {
        if ( getTokenType(1) == TokenType::LPAREN )
        {
            res = std::make_unique<CallNode>(std::move(res), call_params_list());
        }
        else if ( getTokenType(1) == TokenType::DOT )
        {
            match(TokenType::DOT);
            std::string member_name = id();

            res = std::make_unique<DotNode>(std::move(res), member_name);
        }
        else if ( getTokenType(1) == TokenType::LBRACKET )
        {
            match(TokenType::LBRACKET);
            res = std::make_unique<BracketNode>(std::move(res), expression());
            match(TokenType::RBRACKET);
        }
    }

    return res;
}

ASTExprNode Parser::addr_expr()
{
    if ( getTokenType(1) == TokenType::REF )
    {
        match(TokenType::REF);
        return std::make_unique<AddrNode>(unary_right(), AddrOp::REF);
    }
    else
    {
        match(TokenType::MUL);
        return std::make_unique<AddrNode>(unary_right(), AddrOp::DEREF);
    }
}

ASTExprNode Parser::unary_left()
{
    UnaryOp op;

    switch ( getTokenType(1) )
    {
        case TokenType::PLUS : op = UnaryOp::PLUS ; break;
        case TokenType::MINUS: op = UnaryOp::MINUS; break;
        case TokenType::NOT  : op = UnaryOp::NOT  ; break;

        case TokenType::REF: return addr_expr();
        case TokenType::MUL: return addr_expr();
        default            : return unary_right();
    }

    match(getTokenType(1));

    return std::make_unique<UnaryNode>(unary_left(), op);
}

ASTExprNode Parser::factor()
{
    return unary_left();
}

ASTExprNode Parser::term()
{
    auto res = factor();

    while ( getTokenType(1) == TokenType::MUL || getTokenType(1) == TokenType::DIV || getTokenType(1) == TokenType::MOD )
    {
        BinaryOp op;
        switch ( getTokenType(1) )
        {
            case TokenType::MUL: op = BinaryOp::MUL; break;
            case TokenType::DIV: op = BinaryOp::DIV; break;
            case TokenType::MOD: op = BinaryOp::MOD; break;
            default: ;
        }

        match(getTokenType(1));
        res = std::make_unique<BinaryOperatorNode>(std::move(res), factor(), op);
    }

    return res;
}

ASTExprNode Parser::sum_expr()
{
    auto res = term();

    while ( getTokenType(1) == TokenType::PLUS || getTokenType(1) == TokenType::MINUS )
    {
        BinaryOp op;

        switch ( getTokenType(1) )
        {
            case TokenType::PLUS : op = BinaryOp::PLUS; break;
            case TokenType::MINUS: op = BinaryOp::MINUS; break;
            default: ;
        }

        match(getTokenType(1));
        res = std::make_unique<BinaryOperatorNode>(std::move(res), term(), op);
    }

    return res;
}

ASTExprNode Parser::relation()
{
    auto res = sum_expr();

    while ( getTokenType(1) == TokenType::EQUALS || getTokenType(1) == TokenType::NEQUALS )
    {
        BinaryOp op;

        switch ( getTokenType(1) )
        {
            case TokenType::EQUALS : op = BinaryOp::EQUALS; break;
            case TokenType::NEQUALS: op = BinaryOp::NEQUALS; break;
            default: ;
        }
        match(getTokenType(1));
        res = std::make_unique<BinaryOperatorNode>(std::move(res), sum_expr(), op);
    }

    return res;
}

ASTExprNode Parser::bool_expr()
{
    auto res = relation();

    while ( getTokenType(1) == TokenType::AND || getTokenType(1) == TokenType::OR )
    {
        BinaryOp op;

        switch ( getTokenType(1) )
        {
            case TokenType::AND : op = BinaryOp::AND; break;
            case TokenType::OR: op = BinaryOp::OR; break;
            default: ;
        }
        match(getTokenType(1));
        res = std::make_unique<BinaryOperatorNode>(std::move(res), relation(), op);
    }

    return res;
}

ASTExprNode Parser::expression()
{
    return bool_expr();
}

ASTNode Parser::assignment()
{
    auto lhs = expression();
    match(TokenType::ASSIGN);
    auto rhs = expression();

    return std::make_unique<BinaryOperatorNode>(std::move(lhs), std::move(rhs), BinaryOp::ASSIGN);
}

ASTNode Parser::return_stat()
{
    match(TokenType::RETURN);
    return std::make_unique<ReturnNode>(expression());
}

ASTNode Parser::if_stat()
{
    match(TokenType::IF);

    match(TokenType::LPAREN);
    auto cond = expression();
    match(TokenType::RPAREN);

    auto stats_true  = ASTNode{ };
    auto stats_false = ASTNode{ };

    pushScope();
    stats_true = statement();
    popScope();

    if ( getTokenType(1) == TokenType::ELSE )
    {
        match(TokenType::ELSE);
        pushScope();
        stats_false = statement();
        popScope();
    }
    else
        stats_false = std::make_unique<StatementNode>(std::vector< ASTNode >{ });

    return std::make_unique<IfNode>(std::move(cond), std::move(stats_true), std::move(stats_false));
}

ASTNode Parser::while_stat()
{
    match(TokenType::WHILE);

    match(TokenType::LPAREN);
    auto cond = expression();
    match(TokenType::RPAREN);

    is_in_loop.push(true);

    pushScope();
    auto stats = statement();
    popScope();

    is_in_loop.pop();

    return std::make_unique<WhileNode>(std::move(cond), std::move(stats));
}

bool Parser::tryAssignment()
{
    bool success = true;

    mark();

    try
    {
        assignment();
    }
    catch ( RecognitionError& re )
    {
        success = false;
    }

    release();

    return success;
}

ASTNode Parser::for_stat()
{
    match(TokenType::FOR);
    match(TokenType::LPAREN);

    pushScope();

    auto init = ASTNode{ };

    if ( getTokenType(1) == TokenType::SEMICOLON ) init = std::make_unique<StatementNode>(std::vector< ASTNode >{ });
    else if ( tryVarDecl() )                       init = variableDecl();
    else if ( tryAssignment() )                    init = assignment();
    else                                           throw RecognitionError("Expected declaration or expression", getToken(1).line, getToken(1).symbol);

    match(TokenType::SEMICOLON);

    auto cond = (getTokenType(1) == TokenType::SEMICOLON ? std::make_unique<NumberNode>("1") : expression());
    match(TokenType::SEMICOLON);

    auto step = ASTNode{ };

    if ( getTokenType(1) == TokenType::RPAREN ) step = std::make_unique<StatementNode>(std::vector< ASTNode >{ });
    else if ( tryAssignment() )                 step = assignment();
    else                                        step = expression();

    match(TokenType::RPAREN);

    is_in_loop.push(true);
    auto stats = statement();
    is_in_loop.pop();

    popScope();

    return std::make_unique<ForNode>(std::move(init), std::move(cond), std::move(step), std::move(stats));
}

TemplateArgumentInfo Parser::templateArgumentInfo()
{
    if ( tryTypeInfo() ) {
        return typeInfo();
    }
    else {
        return expression();
    }
}

TypeInfo Parser::typeInfo()
{
    bool is_const = false;

    if ( getTokenType(1) == TokenType::CONST )
    {
        is_const = true;
        match(TokenType::CONST);
    }

    auto module_name = std::string("");
    auto type_name   = std::string("");

    if ( tryModuleName() )
    {
        module_name = id();
        match(TokenType::DOT);
        type_name = id();
    }
    else
    {
        type_name = id();
        
        boost::optional<SymbolType_> sym_type = boost::none;
        for ( auto it = symbol_table_stack.rbegin(); it != symbol_table_stack.rend(); ++it )
        {
            auto map_it = it -> find(type_name);

            if ( map_it != std::end(*it) && (map_it -> second == SymbolType_::STRUCT || map_it -> second == SymbolType_::TEMPLATESTRUCT) )
            {
                sym_type = SymbolType_::STRUCT;
                break;
            }            
        }

        if ( !isSpeculating() )
        {
            if ( !sym_type || (sym_type != SymbolType_::STRUCT && sym_type != SymbolType_::TEMPLATESTRUCT) )
                throw SemanticError("'" + type_name + "' is not a type name");
        }

    }

    auto template_arguments = TemplateArgumentsInfo{ };

    if ( getTokenType(1) == TokenType::LESS )
    {
        match(TokenType::LESS);

        if ( getTokenType(1) != TokenType::GREATER )
        {
            template_arguments.emplace_back(templateArgumentInfo());
            while ( getTokenType(1) == TokenType::COMMA )
            {
                match(TokenType::COMMA);
                template_arguments.emplace_back(templateArgumentInfo());
            }
        }

        match(TokenType::GREATER);
    }

    auto modifiers = std::vector<TypeModifier>{ };

    while ( getTokenType(1) == TokenType::MUL || getTokenType(1) == TokenType::LBRACKET )
    {
        switch ( getTokenType(1) )
        {
            case TokenType::MUL: 
            {
                match(TokenType::MUL);
                modifiers.emplace_back(); 
                break;
            }
            case TokenType::LBRACKET: 
            {
                match(TokenType::LBRACKET);
                modifiers.emplace_back(expression());
                match(TokenType::RBRACKET);
                break;
            }
            default: throw "";
        }
    }    

    bool is_ref = false;
    if ( getTokenType(1) == TokenType::REF )
    {
        is_ref = true;
        match(TokenType::REF);
    }

    return TypeInfo(type_name, is_ref, is_const, template_arguments, modifiers, module_name);
}

ParamInfo Parser::paramInfo()
{
    auto type_info = typeInfo();
    auto name      = id();

    return {name, type_info};
}

std::vector<ASTExprNode> Parser::call_params_list()
{
    auto params = std::vector<ASTExprNode>{ };

    match(TokenType::LPAREN);

    if ( getTokenType(1) != TokenType::RPAREN )
    {
        params.push_back(expression());
        while ( getTokenType(1) == TokenType::COMMA )
        {
            match(TokenType::COMMA);
            params.push_back(expression());
        }
    }

    match(TokenType::RPAREN);

    return params;
}

ASTExprNode Parser::new_expr()
{
    match(TokenType::NEW);

    auto type_info = typeInfo();

    auto params = std::vector<ASTExprNode>{ };

    if ( getTokenType(1) == TokenType::LPAREN )
        params = call_params_list();

    return std::make_unique<NewExpressionNode>(std::move(type_info), std::move(params));
}

ASTNode Parser::import_stat()
{
    match(TokenType::IMPORT);

    auto module_name = id();
    rememberSymbol(module_name, SymbolType_::MODULE);
    
    const auto& unit = Comp::compile(module_name);

    return std::make_unique<ImportNode>(module_name, unit.root.get(), std::vector<Symbol*>{ });
}

bool Parser::tryTypeInfo()
{
    bool success = true;

    mark();

    try
    {
        typeInfo();
    }
    catch ( RecognitionError& e )
    {
        success = false;
    }

    release();

    return success;
}

bool Parser::tryModuleName()
{
    bool success = true;

    mark();

    try
    {
        auto str = id();
        auto sym_type = resolveSymbolType(str);

        if ( !sym_type || *sym_type != SymbolType_::MODULE )
            throw RecognitionError("", 0, 0);
    }
    catch ( RecognitionError& e )
    {
        success = false;        
    }

    release();
    
    return success;
}

std::unique_ptr<DeclarationNode> Parser::varInferDecl(boost::optional<std::string>)
{
    match(TokenType::VAR);

    auto name = id();

    match(TokenType::ASSIGN);

    auto expr = expression();

    rememberSymbol(name, SymbolType_::VARIABLE);

    return std::make_unique<VarInferTypeDeclarationNode>(std::move(name), std::move(expr));
}

ASTNode Parser::unsafe_block()
{
    match(TokenType::UNSAFE);

    pushScope();
    auto stat = block();
    popScope();

    return std::make_unique<UnsafeBlockNode>(std::move(stat));
}

ASTNode Parser::extern_stat()
{
    match(TokenType::EXTERN);

    bool is_unsafe = false;

    if ( getTokenType(1) == TokenType::UNSAFE )
    {
        is_unsafe = true;
        match(TokenType::UNSAFE);
    }

    auto function_name = (getTokenType(1) == TokenType::OPERATOR ? operator_name() : id());
    auto params = formalParams();

    TypeInfo return_type_info;

    if ( getTokenType(1) == TokenType::COLON )
    {
        match(TokenType::COLON);
        return_type_info = typeInfo();
    }
    else
        return_type_info = TypeInfo("void", false, false);

    rememberSymbol(function_name, SymbolType_::FUNCTION);

    return std::make_unique<ExternNode>(std::move(function_name), std::move(FunctionDeclarationInfo(return_type_info, params)), is_unsafe);
}
    
ASTNode Parser::from_import_stat()
{
    match(TokenType::FROM);
    auto module_name = id();

    const auto& unit = Comp::compile(module_name);

    match(TokenType::IMPORT);
    auto member_name = id();

    auto it = std::find_if(std::begin(unit.module_globals), std::end(unit.module_globals), [&](auto&& sym) 
    { 
        return sym -> getName() == member_name; 
    });

    rememberSymbol(member_name, getSymbolType(*it)); 

    return std::make_unique<ImportNode>(module_name, unit.root.get(), std::vector<Symbol*>{*it});
}

ASTNode Parser::break_stat()
{
    if ( !is_in_loop.top() )
        throw RecognitionError("Break not in loop", getToken(1).line, getToken(1).symbol);
    match(TokenType::BREAK);
    return std::make_unique<BreakNode>();
}
