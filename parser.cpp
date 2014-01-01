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
    else if ( tryAssignment() )
	return assignment();
    else
	return expression();
}

DeclarationNode* Parser::declaration()
{
    if ( getTokenType(1) == TokenType::STRUCT )
	return structDecl();
    else if ( getTokenType(1) == TokenType::DEF )
	return functionDecl();
    else
	return variableDecl();
}

DeclarationNode* Parser::variableDecl()
{
    match(TokenType::VAR);

    string variable_name = id();

    match(TokenType::COLON);

    string type_name = id();

    return new VariableDeclarationNode(variable_name, type_name);
}

DeclarationNode* Parser::structDecl()
{
    match(TokenType::STRUCT);

    string struct_name = id();

    match(TokenType::LBRACE);

    vector<DeclarationNode*> struct_in;
    
    while ( getTokenType(1) != TokenType::RBRACE )
	struct_in.push_back(declaration());
    
    match(TokenType::RBRACE);

    return new StructDeclarationNode(struct_name, struct_in);
}

//def a(x : int, y : int) : int { }
DeclarationNode* Parser::functionDecl()
{
    match(TokenType::DEF);

    string function_name = id();

    vector< std::pair<string, string> > params;
    
    match(TokenType::LPAREN);

    if ( getTokenType(1) != TokenType::RPAREN )
    {
	string param_name, type_name;

	param_name = id();
	match(TokenType::COLON);
	type_name = id();

	params.push_back({param_name, type_name});
	
	while ( getTokenType(1) != TokenType::RPAREN )
	{
	    match(TokenType::COMMA);
	    
	    param_name = id();
	    match(TokenType::COLON);
	    type_name = id();		    

	    params.push_back({param_name, type_name});
	}
    }

    match(TokenType::RPAREN);

    match(TokenType::COLON);

    string return_type_name = id();    

    vector < AST* > statements;
    
    match(TokenType::LBRACE);

    while ( getTokenType(1) != TokenType::RBRACE )
	statements.push_back(statement());

    match(TokenType::RBRACE);

    return new FunctionDeclarationNode(function_name, params, return_type_name, statements);
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
    else
	return variable();        
}

ExprNode* Parser::unary_right()
{
    ExprNode *res = primary();

    while ( getTokenType(1) == TokenType::LPAREN )
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

	res = new CallNode(res, params);
    }

    return res;
}

ExprNode* Parser::factor()
{
    return unary_right();
}

ExprNode* Parser::expression()
{
    return factor();
}

AST* Parser::assignment()
{
    ExprNode *lhs = expression();
    match(TokenType::ASSIGN);
    ExprNode *rhs = expression();

    return new AssignmentNode(lhs, rhs);
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
