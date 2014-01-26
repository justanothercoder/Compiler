#include "parser.hpp"

Parser::Parser(AbstractLexer *lexer) : AbstractParser(lexer)
{
    
}

AST* Parser::parse()
{
    vector<AST*> statements;

    while ( getTokenType(1) != TokenType::EOF_TYPE )
	statements.push_back(statement());

    return new StatementNode(statements);
}

AST* Parser::statement()
{
    if ( getTokenType(1) == TokenType::SEMICOLON )
    {
	match(TokenType::SEMICOLON);
	return new EmptyStatementNode();
    }
    else if ( getTokenType(1) == TokenType::STRUCT || getTokenType(1) == TokenType::DEF || getTokenType(1) == TokenType::VAR )    
	return declaration();
    else if ( getTokenType(1) == TokenType::RETURN )
	return return_stat();
    else if ( getTokenType(1) == TokenType::IF )
	return if_stat();
    else if ( getTokenType(1) == TokenType::WHILE )
	return while_stat();
    else if ( tryAssignment() )
	return assignment();
    else
	return expression();
}

DeclarationNode* Parser::declaration(std::shared_ptr<string> struct_name)
{
    if ( getTokenType(1) == TokenType::STRUCT )
	return structDecl();
    else if ( getTokenType(1) == TokenType::DEF )
	return functionDecl(struct_name);
    else
	return variableDecl(struct_name);
}

DeclarationNode* Parser::variableDecl(std::shared_ptr<string> struct_name)
{
    match(TokenType::VAR);
    auto var = var_and_type();

    vector<ExprNode*> constructor_call_params;
    
    if ( getTokenType(1) == TokenType::LPAREN )
	constructor_call_params = call_params_list();
    
    return new VariableDeclarationNode(var.first, var.second, struct_name != nullptr, constructor_call_params);
}

DeclarationNode* Parser::structDecl()
{
    match(TokenType::STRUCT);

    string struct_name = id();

    match(TokenType::LBRACE);

    vector<DeclarationNode*> struct_in;
    
    while ( getTokenType(1) != TokenType::RBRACE )
    {
	while ( getTokenType(1) == TokenType::SEMICOLON )
	    match(TokenType::SEMICOLON);

	if ( getTokenType(1) != TokenType::RBRACE )
	    struct_in.push_back(declaration(std::make_shared<string>(string(struct_name))));
    }
    
    match(TokenType::RBRACE);

    return new StructDeclarationNode(struct_name, struct_in);
}

DeclarationNode* Parser::functionDecl(std::shared_ptr<string> struct_name)
{
    match(TokenType::DEF);

    string function_name = id();

    vector< std::pair<string, TypeInfo> > params;
    
    match(TokenType::LPAREN);

    if ( getTokenType(1) != TokenType::RPAREN )
    {
	params.push_back(var_and_type());
	
	while ( getTokenType(1) != TokenType::RPAREN )
	{
	    match(TokenType::COMMA);	   
	    params.push_back(var_and_type());
	}
    }

    match(TokenType::RPAREN);

    TypeInfo return_type;

    bool is_constructor = (struct_name != nullptr && function_name == *struct_name);
    
    if ( !is_constructor )
    {
	if ( getTokenType(1) == TokenType::COLON )
	{
	    match(TokenType::COLON);
	    return_type = type_info();
	}
	else
	{
	    return_type = TypeInfo("void", false);
	}
    }
    else
    {
	return_type = TypeInfo(*struct_name, true);
    }

    vector < AST* > statements;
    
    match(TokenType::LBRACE);

    while ( getTokenType(1) != TokenType::RBRACE )
	statements.push_back(statement());

    match(TokenType::RBRACE);

    return new FunctionDeclarationNode(function_name, params, return_type, statements, {struct_name != nullptr, is_constructor, false});
}

string Parser::id()
{
    string name = getToken(1).text;
    match(TokenType::ID);
    return name;
}

ExprNode* Parser::literal()
{
    return number();
}

ExprNode* Parser::variable()
{
    string name = id();
    return new VariableNode(name);
}

ExprNode* Parser::number()
{
    string num = getToken(1).text;
    match(TokenType::NUMBER);
    return new NumberNode(num);
}

ExprNode* Parser::primary()
{
    if ( getTokenType(1) == TokenType::NUMBER )
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
    else
	return variable();        
}

ExprNode* Parser::unary_right()
{
    ExprNode *res = primary();

    while ( getTokenType(1) == TokenType::LPAREN || getTokenType(1) == TokenType::DOT )
    {
	if ( getTokenType(1) == TokenType::LPAREN )
	    res = new CallNode(res, call_params_list());
	else
	{
	    match(TokenType::DOT);
	    string member_name = id();

	    res = new DotNode(res, member_name);
	}
    }

    return res;
}

ExprNode* Parser::factor()
{
    return unary_right();
}

ExprNode* Parser::term()
{
    ExprNode *res = factor();

    while ( getTokenType(1) == TokenType::MUL )
    {
	if ( getTokenType(1) == TokenType::MUL )
	{
	    match(TokenType::MUL);
	    res = new BinaryOperatorNode(res, factor(), BinaryOp::MUL);
	}
    }

    return res;
}

ExprNode* Parser::sum_expr()
{
    ExprNode *res = term();

    while ( getTokenType(1) == TokenType::PLUS || getTokenType(1) == TokenType::MINUS )
    {
	if ( getTokenType(1) == TokenType::PLUS )
	{
	    match(TokenType::PLUS);
	    res = new BinaryOperatorNode(res, term(), BinaryOp::PLUS);
	}
	else if ( getTokenType(1) == TokenType::MINUS )
	{
	    match(TokenType::MINUS);
	    res = new BinaryOperatorNode(res, term(), BinaryOp::MINUS);
	}
    }

    return res;
}

ExprNode* Parser::expression()
{
    return sum_expr();
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
    
    if ( getTokenType(1) == TokenType::LBRACE )
    {
	match(TokenType::LBRACE);

	vector<AST*> statements;

	while ( getTokenType(1) != TokenType::RBRACE )
	    statements.push_back(statement());
	
	stats_true = new StatementNode(statements);
	match(TokenType::RBRACE);
    }
    else
    {
	stats_true = new StatementNode({statement()});
    }

    if ( getTokenType(1) == TokenType::ELSE )
    {
	match(TokenType::ELSE);
	if ( getTokenType(1) == TokenType::LBRACE )
	{
	    match(TokenType::LBRACE);

	    vector<AST*> statements;

	    while ( getTokenType(1) != TokenType::RBRACE )
		statements.push_back(statement());
	
	    stats_false = new StatementNode(statements);
	    match(TokenType::RBRACE);
	}
	else
	{
	    stats_false = new StatementNode({statement()});
	}
    }
    else
    {
	stats_false = new StatementNode({});
    }

    return new IfNode(cond, stats_true, stats_false);
}

AST* Parser::while_stat()
{
    match(TokenType::WHILE);

    match(TokenType::LPAREN);
    ExprNode *cond = expression();
    match(TokenType::RPAREN);

    AST *stats;
    
    if ( getTokenType(1) == TokenType::LBRACE )
    {
	match(TokenType::LBRACE);

	vector<AST*> statements;

	while ( getTokenType(1) != TokenType::RBRACE )
	    statements.push_back(statement());
	
	stats = new StatementNode(statements);
	match(TokenType::RBRACE);
    }
    else
    {
	stats = new StatementNode({statement()});
    }

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

pair<string, TypeInfo> Parser::var_and_type()
{
    string name = id();

    match(TokenType::COLON);

    return {name, type_info()};
}

TypeInfo Parser::type_info()
{
    string type_name = id();

    bool is_ref = false;
    
    if ( getTokenType(1) == TokenType::REF )
    {
	is_ref = true;
	match(TokenType::REF);
    }

    return TypeInfo(type_name, is_ref);
}

vector<ExprNode*> Parser::call_params_list()
{
    vector<ExprNode*> params;
	
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

ExprNode* Parser::new_expr()
{
    match(TokenType::NEW);

    string name = id();
    
    vector<ExprNode*> params = { };

    if ( getTokenType(1) == TokenType::LPAREN )
	params = call_params_list();

    return new NewExpressionNode(name, params);
}
