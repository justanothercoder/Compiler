#include "parser.hpp"
#include "semanticerror.hpp"
#include "breaknode.hpp"
#include "functionnode.hpp"
#include "typenode.hpp"
#include "modulenode.hpp"
#include "filehelper.hpp"
#include "modulememberaccessnode.hpp"
#include "compilableunit.hpp"
#include "comp.hpp"

Parser::Parser(AbstractLexer *lexer) : AbstractParser(lexer)
{

}
    
void Parser::pushScope()
{
    symbol_table_stack.emplace_back();
}

void Parser::popScope()
{
    symbol_table_stack.pop_back();
}
    
void Parser::rememberSymbol(std::string name, SymbolType type)
{
    auto& table = symbol_table_stack.back();

    auto it = table.find(name);

    if ( it == std::end(table) )
        table[name] = type;
    else
    {
        if ( !(it -> second == SymbolType::FUNCTION && type == SymbolType::FUNCTION) )
            throw SemanticError("Symbol " + name + " is already defined");
    }
}
    
boost::optional<SymbolType> Parser::resolveSymbolType(std::string name)
{
    boost::optional<SymbolType> sym_type = boost::none;
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

AST* Parser::parse()
{
    pushScope();
    
    is_in_loop.push(false);

    rememberSymbol("int", SymbolType::STRUCT);
    rememberSymbol("void", SymbolType::STRUCT);
    rememberSymbol("char", SymbolType::STRUCT);
    rememberSymbol("class", SymbolType::STRUCT);
    rememberSymbol("putchar", SymbolType::FUNCTION);
    rememberSymbol("print", SymbolType::FUNCTION);
    rememberSymbol("string", SymbolType::STRUCT);

    std::vector<AST*> statements;

    while ( getTokenType(1) != TokenType::EOF_TYPE )
        statements.push_back(statement());

    popScope();

    return new StatementNode(std::move(statements));
}

AST* Parser::statement()
{
    if ( getTokenType(1) == TokenType::SEMICOLON )
    {
        match(TokenType::SEMICOLON);
        return new StatementNode({ });
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
    else if ( tryAssignment() )                      return assignment();
    else if ( getTokenType(1) == TokenType::LBRACE ) return block();
    else if ( getTokenType(1) == TokenType::IMPORT ) return import_stat();
    else if ( getTokenType(1) == TokenType::UNSAFE ) return unsafe_block();
    else if ( getTokenType(1) == TokenType::EXTERN ) return extern_stat();
    else if ( getTokenType(1) == TokenType::FROM   ) return from_import_stat();
    else if ( getTokenType(1) == TokenType::BREAK  ) return break_stat();
    else                                             return expression();
}

AST* Parser::block()
{
    match(TokenType::LBRACE);

    std::vector<AST*> statements;

    while ( getTokenType(1) != TokenType::RBRACE )
        statements.push_back(statement());

    match(TokenType::RBRACE);

    return new StatementNode(std::move(statements));
}

DeclarationNode* Parser::declaration(boost::optional<std::string> struct_name)
{
    if      ( getTokenType(1) == TokenType::STRUCT )   return structDecl();
    else if ( getTokenType(1) == TokenType::TEMPLATE ) return templateStructDecl();
    else if ( getTokenType(1) == TokenType::DEF )      return functionDecl(struct_name);
    else if ( getTokenType(1) == TokenType::VAR )      return varInferDecl(struct_name);
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

DeclarationNode* Parser::variableDecl(boost::optional<std::string> struct_name)
{
    auto type_info = typeInfo();
    std::string var_name  = id();

    std::vector<ExprNode*> constructor_call_params;

    if ( getTokenType(1) == TokenType::LPAREN )
        constructor_call_params = call_params_list();
    else if ( getTokenType(1) == TokenType::ASSIGN )
    {
        match(TokenType::ASSIGN);
        constructor_call_params = {expression()};
    }

    rememberSymbol(var_name, SymbolType::VARIABLE);

    return new VariableDeclarationNode(std::move(var_name), std::move(type_info), bool(struct_name), std::move(constructor_call_params));
}

DeclarationNode* Parser::structDecl()
{
    match(TokenType::STRUCT);

    std::string struct_name = id();

    std::vector<AST*> struct_in;
    match(TokenType::LBRACE);

    std::vector<AST*> functions;

    rememberSymbol(struct_name, SymbolType::STRUCT);
    pushScope();
    rememberSymbol("this", SymbolType::VARIABLE);

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

    for ( auto i : functions )
        struct_in.push_back(i);

    popScope();

    return new StructDeclarationNode(std::move(struct_name), std::move(struct_in));
}

DeclarationNode* Parser::templateStructDecl()
{
    match(TokenType::TEMPLATE);
    match(TokenType::LESS);

    std::vector< std::pair<std::string, TypeInfo> > template_params;

    if ( getTokenType(1) != TokenType::GREATER )
    {
        auto type_info = typeInfo();
        auto name = id();

        if ( type_info.type_name == "class" )
            rememberSymbol(name, SymbolType::STRUCT);
        else
            rememberSymbol(name, SymbolType::VARIABLE);

        template_params.push_back({std::move(name), std::move(type_info)});
        while ( getTokenType(1) == TokenType::COMMA )
        {
            match(TokenType::COMMA);

            type_info = typeInfo();
            name = id();

            if ( type_info.type_name == "class" )
                rememberSymbol(name, SymbolType::STRUCT);
            else
                rememberSymbol(name, SymbolType::VARIABLE);

            template_params.push_back({std::move(name), std::move(type_info)});
        }
    }

    match(TokenType::GREATER);

    match(TokenType::STRUCT);

    std::string struct_name = id();

    if ( getTokenType(1) == TokenType::LESS )
    {
        match(TokenType::LESS);

        match(TokenType::GREATER);
    }

    std::vector<AST*> struct_in;
    match(TokenType::LBRACE);

    rememberSymbol(struct_name, SymbolType::TEMPLATESTRUCT);
    pushScope();

    while ( getTokenType(1) != TokenType::RBRACE )
    {
        while ( getTokenType(1) == TokenType::SEMICOLON )
            match(TokenType::SEMICOLON);

        if ( getTokenType(1) != TokenType::RBRACE )
            struct_in.push_back(declaration(struct_name));
    }

    match(TokenType::RBRACE);

    popScope();

    return new TemplateStructDeclarationNode(std::move(struct_name), std::move(struct_in), std::move(template_params));
}

DeclarationNode* Parser::functionDecl(boost::optional<std::string> struct_name)
{
    match(TokenType::DEF);

    bool is_unsafe = false;

    if ( getTokenType(1) == TokenType::UNSAFE )
    {
        is_unsafe = true;
        match(TokenType::UNSAFE);
    }

    bool is_operator = getTokenType(1) == TokenType::OPERATOR;

    std::string function_name = (is_operator ? operator_name() : id());
    
    rememberSymbol(function_name, SymbolType::FUNCTION);
    pushScope();

    std::vector< std::pair<std::string, TypeInfo> > params;

    match(TokenType::LPAREN);

    if ( getTokenType(1) != TokenType::RPAREN )
    {
        auto type_info = typeInfo();
        auto name = id();

        rememberSymbol(name, SymbolType::VARIABLE);

        params.push_back({std::move(name), std::move(type_info)});

        while ( getTokenType(1) != TokenType::RPAREN )
        {
            match(TokenType::COMMA);

            type_info = typeInfo();
            name = id();

            rememberSymbol(name, SymbolType::VARIABLE);

            params.push_back({std::move(name), std::move(type_info)});
        }
    }

    match(TokenType::RPAREN);

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

    AST *statements = block();

    popScope();

    return new FunctionDeclarationNode(std::move(function_name)
                                     , std::move(params)
                                     , std::move(return_type)
                                     , statements
                                     , {bool(struct_name), is_constructor, is_operator}
                                     , is_unsafe
                                    );
}

std::string Parser::id()
{
    std::string name = getToken(1).text;
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

ExprNode* Parser::literal()
{
    if ( getTokenType(1) == TokenType::STRING )
        return get_string();
    else
        return number();
}

ExprNode* Parser::variable()
{
    auto var_name = id();

    auto sym_type = resolveSymbolType(var_name);

    if ( !sym_type )
        throw SemanticError("No such symbol " + var_name);

    switch ( *sym_type )
    {
    case SymbolType::VARIABLE: return new VariableNode(var_name);
    case SymbolType::FUNCTION: return new FunctionNode(var_name);
    case SymbolType::MODULE  : return new ModuleNode(var_name);
    case SymbolType::STRUCT  : return new TypeNode(var_name);
    default: throw std::logic_error("Internal error");
    }
}

ExprNode* Parser::get_string()
{
    std::string str = getToken(1).text;
    match(TokenType::STRING);
    return new StringNode(str);
}

ExprNode* Parser::number()
{
    std::string num = getToken(1).text;
    match(TokenType::NUMBER);
    return new NumberNode(num);
}

ExprNode* Parser::null()
{
    match(TokenType::NULLTOKEN);
    return new NullNode();
}

ExprNode* Parser::primary()
{
    if ( getTokenType(1) == TokenType::NUMBER || getTokenType(1) == TokenType::STRING )
        return literal();
    else if ( getTokenType(1) == TokenType::LPAREN )
    {
        match(TokenType::LPAREN);
        ExprNode *expr = expression();
        match(TokenType::RPAREN);
        return expr;
    }
    else if ( getTokenType(1) == TokenType::NEW )
        return new_expr();
    else if ( getTokenType(1) == TokenType::NULLTOKEN )
        return null();
    else
        return variable();
}

ExprNode* Parser::unary_right()
{
    ExprNode* res;

    if ( tryModuleName() )
    {
        auto module_name = id();
        match(TokenType::DOT);
        auto member = id();
        res = new ModuleMemberAccessNode(module_name, member);
    }
    else
    {
        res = primary();
    }

    while ( getTokenType(1) == TokenType::LPAREN || getTokenType(1) == TokenType::DOT || getTokenType(1) == TokenType::LBRACKET )
    {
        if ( getTokenType(1) == TokenType::LPAREN )
        {
            res = new CallNode(res, call_params_list());
        }
        else if ( getTokenType(1) == TokenType::DOT )
        {
            match(TokenType::DOT);
            string member_name = id();

            res = new DotNode(res, member_name);
        }
        else if ( getTokenType(1) == TokenType::LBRACKET )
        {
            match(TokenType::LBRACKET);
            res = new BracketNode(res, expression());
            match(TokenType::RBRACKET);
        }
    }

    return res;
}

ExprNode* Parser::addr_expr()
{
    if ( getTokenType(1) == TokenType::REF )
    {
        match(TokenType::REF);
        return new AddrNode(unary_right(), AddrOp::REF);
    }
    else
    {
        match(TokenType::MUL);
        return new AddrNode(unary_right(), AddrOp::DEREF);
    }
}

ExprNode* Parser::unary_left()
{
    UnaryOp op;

    switch ( getTokenType(1) )
    {
    case TokenType::PLUS :
        op = UnaryOp::PLUS ;
        break;
    case TokenType::MINUS:
        op = UnaryOp::MINUS;
        break;
    case TokenType::NOT  :
        op = UnaryOp::NOT  ;
        break;

    case TokenType::REF:
        return addr_expr();
    case TokenType::MUL:
        return addr_expr();
    default            :
        return unary_right();
    }

    match(getTokenType(1));

    return new UnaryNode(unary_left(), op);
}

ExprNode* Parser::factor()
{
    return unary_left();
}

ExprNode* Parser::term()
{
    ExprNode *res = factor();

    while ( getTokenType(1) == TokenType::MUL || getTokenType(1) == TokenType::DIV || getTokenType(1) == TokenType::MOD )
    {
        BinaryOp op;
        switch ( getTokenType(1) )
        {
        case TokenType::MUL:
            op = BinaryOp::MUL;
            break;
        case TokenType::DIV:
            op = BinaryOp::DIV;
            break;
        case TokenType::MOD:
            op = BinaryOp::MOD;
            break;
        default:
            throw;
        }

        match(getTokenType(1));
        res = new BinaryOperatorNode(res, factor(), op);
    }

    return res;
}

ExprNode* Parser::sum_expr()
{
    ExprNode *res = term();

    while ( getTokenType(1) == TokenType::PLUS || getTokenType(1) == TokenType::MINUS )
    {
        BinaryOp op;

        switch ( getTokenType(1) )
        {
        case TokenType::PLUS :
            op = BinaryOp::PLUS;
            break;
        case TokenType::MINUS:
            op = BinaryOp::MINUS;
            break;
        default:
            throw;
        }

        match(getTokenType(1));
        res = new BinaryOperatorNode(res, term(), op);
    }

    return res;
}

ExprNode* Parser::relation()
{
    ExprNode *res = sum_expr();

    while ( getTokenType(1) == TokenType::EQUALS || getTokenType(1) == TokenType::NEQUALS )
    {
        BinaryOp op;

        switch ( getTokenType(1) )
        {
        case TokenType::EQUALS :
            op = BinaryOp::EQUALS;
            break;
        case TokenType::NEQUALS:
            op = BinaryOp::NEQUALS;
            break;
        default:
            throw;
        }
        match(getTokenType(1));
        res = new BinaryOperatorNode(res, sum_expr(), op);
    }

    return res;
}

ExprNode* Parser::bool_expr()
{
    ExprNode *res = relation();

    while ( getTokenType(1) == TokenType::AND || getTokenType(1) == TokenType::OR )
    {
        BinaryOp op;

        switch ( getTokenType(1) )
        {
        case TokenType::AND :
            op = BinaryOp::AND;
            break;
        case TokenType::OR:
            op = BinaryOp::OR;
            break;
        default:
            throw;
        }
        match(getTokenType(1));
        res = new BinaryOperatorNode(res, relation(), op);
    }

    return res;
}

ExprNode* Parser::expression()
{
    return bool_expr();
}

AST* Parser::assignment()
{
    ExprNode *lhs = expression();
    match(TokenType::ASSIGN);
    ExprNode *rhs = expression();

    return new BinaryOperatorNode(lhs, rhs, BinaryOp::ASSIGN);
}

AST* Parser::return_stat()
{
    match(TokenType::RETURN);

    ExprNode *expr = expression();
    return new ReturnNode(expr);
}

AST* Parser::if_stat()
{
    match(TokenType::IF);

    match(TokenType::LPAREN);
    ExprNode *cond = expression();
    match(TokenType::RPAREN);

    AST *stats_true, *stats_false;

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
        stats_false = new StatementNode({ });

    return new IfNode(cond, stats_true, stats_false);
}

AST* Parser::while_stat()
{
    match(TokenType::WHILE);

    match(TokenType::LPAREN);
    ExprNode *cond = expression();
    match(TokenType::RPAREN);

    is_in_loop.push(true);

    pushScope();
    AST *stats = statement();
    popScope();

    is_in_loop.pop();

    return new WhileNode(cond, stats);
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

AST* Parser::for_stat()
{
    match(TokenType::FOR);
    match(TokenType::LPAREN);

    pushScope();

    AST *init;

    if ( getTokenType(1) == TokenType::SEMICOLON ) init = new StatementNode({ });
    else if ( tryVarDecl() )                       init = variableDecl();
    else if ( tryAssignment() )                    init = assignment();
    else                                           throw RecognitionError("Expected declaration or expression", getToken(1).line, getToken(1).symbol);

    match(TokenType::SEMICOLON);

    ExprNode *cond;

    if ( getTokenType(1) == TokenType::SEMICOLON )
        cond = new NumberNode("1");
    else
        cond = expression();

    match(TokenType::SEMICOLON);

    AST *step;

    if ( getTokenType(1) == TokenType::RPAREN ) step = new StatementNode({ });
    else if ( tryAssignment() )                 step = assignment();
    else                                        step = expression();

    match(TokenType::RPAREN);

    is_in_loop.push(true);
    AST *stats = statement();
    is_in_loop.pop();

    popScope();

    return new ForNode(init, cond, step, stats);
}

TypeInfo Parser::typeInfo()
{
    bool is_const = false;

    if ( getTokenType(1) == TokenType::CONST )
    {
        is_const = true;
        match(TokenType::CONST);
    }

    std::string module_name;
    std::string type_name;

    if ( tryModuleName() )
    {
        module_name = id();
        match(TokenType::DOT);
        type_name = id();
    }
    else
    {
        module_name = "";
        type_name = id();
    
        boost::optional<SymbolType> sym_type = boost::none;
        for ( auto it = symbol_table_stack.rbegin(); it != symbol_table_stack.rend(); ++it )
        {
            auto map_it = it -> find(type_name);

            if ( map_it != std::end(*it) && (map_it -> second == SymbolType::STRUCT || map_it -> second == SymbolType::TEMPLATESTRUCT) )
            {
                sym_type = SymbolType::STRUCT;
                break;
            }            
        }

        if ( !isSpeculating() )
        {
            if ( !sym_type || (sym_type != SymbolType::STRUCT && sym_type != SymbolType::TEMPLATESTRUCT) )
                throw SemanticError("'" + type_name + "' is not a type name");
        }
    }

    bool is_ref = false;

    std::vector<TemplateParamInfo> template_params { };

    if ( getTokenType(1) == TokenType::LESS )
    {
        match(TokenType::LESS);

        if ( getTokenType(1) != TokenType::GREATER )
        {
            if ( tryTypeInfo() )
                template_params.push_back(typeInfo());
            else
                template_params.push_back(expression());

            while ( getTokenType(1) == TokenType::COMMA )
            {
                match(TokenType::COMMA);
                if ( tryTypeInfo() )
                    template_params.push_back(typeInfo());
                else
                    template_params.push_back(expression());
            }
        }

        match(TokenType::GREATER);
    }

    int pointer_depth = 0;

    while ( getTokenType(1) == TokenType::MUL )
    {
        match(TokenType::MUL);
        ++pointer_depth;
    }
    
    std::vector<ExprNode*> array_dimensions;
    while ( getTokenType(1) == TokenType::LBRACKET )
    {
        match(TokenType::LBRACKET);
        array_dimensions.push_back(expression());
        match(TokenType::RBRACKET);
    }

    if ( getTokenType(1) == TokenType::REF )
    {
        is_ref = true;
        match(TokenType::REF);
    }

    return TypeInfo(std::move(type_name), is_ref, is_const, std::move(template_params), pointer_depth, std::move(array_dimensions), module_name);
}

std::vector<ExprNode*> Parser::call_params_list()
{
    std::vector<ExprNode*> params;

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

    return std::move(params);
}

ExprNode* Parser::new_expr()
{
    match(TokenType::NEW);

    auto type_info = typeInfo();

    std::vector<ExprNode*> params = { };

    if ( getTokenType(1) == TokenType::LPAREN )
        params = call_params_list();

    return new NewExpressionNode(std::move(type_info), std::move(params));
}

AST* Parser::import_stat()
{
    match(TokenType::IMPORT);

    auto module_name = id();
    rememberSymbol(module_name, SymbolType::MODULE);
    
//    auto root = FileHelper::parse((module_name + ".txt").c_str());
    auto unit = Comp::compile(module_name);

    return new ImportNode(module_name, unit.root, { });
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

        if ( !sym_type || *sym_type != SymbolType::MODULE )
            throw RecognitionError("", 0, 0);
    }
    catch ( RecognitionError& e )
    {
        success = false;        
    }

    release();
    
    return success;
}

DeclarationNode* Parser::varInferDecl(boost::optional<string>)
{
    match(TokenType::VAR);

    std::string name = id();

    match(TokenType::ASSIGN);

    auto expr = expression();

    rememberSymbol(name, SymbolType::VARIABLE);

    return new VarInferTypeDeclarationNode(std::move(name), expr);
}

AST* Parser::unsafe_block()
{
    match(TokenType::UNSAFE);

    pushScope();
    auto stat = block();
    popScope();

    return new UnsafeBlockNode(static_cast<StatementNode*>(stat));
}

AST* Parser::extern_stat()
{
    match(TokenType::EXTERN);

    bool is_unsafe = false;

    if ( getTokenType(1) == TokenType::UNSAFE )
    {
        is_unsafe = true;
        match(TokenType::UNSAFE);
    }

    std::string function_name = (getTokenType(1) == TokenType::OPERATOR ? operator_name() : id());

    std::vector< std::pair<std::string, TypeInfo> > params;

    match(TokenType::LPAREN);

    if ( getTokenType(1) != TokenType::RPAREN )
    {
        auto type_info = typeInfo();
        auto name = id();

        params.push_back({std::move(name), std::move(type_info)});

        while ( getTokenType(1) != TokenType::RPAREN )
        {
            match(TokenType::COMMA);

            type_info = typeInfo();
            name = id();
            params.push_back({std::move(name), std::move(type_info)});
        }
    }

    match(TokenType::RPAREN);

    TypeInfo return_type_info;

    if ( getTokenType(1) == TokenType::COLON )
    {
        match(TokenType::COLON);
        return_type_info = typeInfo();
    }
    else
        return_type_info = TypeInfo("void", false, false);

    rememberSymbol(function_name, SymbolType::FUNCTION);

    return new ExternNode(std::move(function_name), std::move(params), std::move(return_type_info), is_unsafe);
}
    
AST* Parser::from_import_stat()
{
    match(TokenType::FROM);
    std::string module_name = id();

    auto unit = Comp::compile(module_name);

//    auto root = FileHelper::parse((module_name + ".txt").c_str());

    match(TokenType::IMPORT);
    std::string member_name = id();

    auto it = std::find_if(std::begin(unit.module_globals), std::end(unit.module_globals), [&](Symbol* sym) 
    { 
        return sym -> getName() == member_name; 
    });

    rememberSymbol(member_name, (*it) -> getSymbolType()); 

    return new ImportNode(module_name, unit.root, {*it}); 
}

AST* Parser::break_stat()
{
    if ( !is_in_loop.top() )
        throw RecognitionError("Break not in loop", getToken(1).line, getToken(1).symbol);
    match(TokenType::BREAK);
    return new BreakNode();
}
