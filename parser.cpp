#include "parser.hpp"

Parser::Parser(AbstractLexer *lexer) : AbstractParser(lexer) { }

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
		return new StatementNode({ });
	}
	else if ( getTokenType(1) == TokenType::STRUCT || getTokenType(1) == TokenType::DEF || tryVarDecl() || getTokenType(1) == TokenType::TEMPLATE )
		return declaration();
	else if ( getTokenType(1) == TokenType::RETURN ) return return_stat();
	else if ( getTokenType(1) == TokenType::IF     ) return if_stat();
	else if ( getTokenType(1) == TokenType::WHILE  ) return while_stat();
	else if ( getTokenType(1) == TokenType::FOR    ) return for_stat();
	else if ( tryAssignment() )                      return assignment();
	else if ( getTokenType(1) == TokenType::LBRACE ) return block();
	else                                             return expression();
}

AST* Parser::block()
{
	match(TokenType::LBRACE);

	vector<AST*> statements;

	while ( getTokenType(1) != TokenType::RBRACE )
		statements.push_back(statement());

	match(TokenType::RBRACE);

	return new StatementNode(statements);
}

DeclarationNode* Parser::declaration(std::shared_ptr<string> struct_name)
{
	if      ( getTokenType(1) == TokenType::STRUCT )   return structDecl();
	else if ( getTokenType(1) == TokenType::TEMPLATE ) return templateStructDecl();
	else if ( getTokenType(1) == TokenType::DEF )      return functionDecl(struct_name);
	else if ( tryVarDecl() )                           return variableDecl(struct_name);
	else                                               throw RecognitionError("Declaration expected.");
}

bool Parser::tryVarDecl()
{
	bool success = true;

	mark();

	try { typeInfo(); id(); }
	catch ( RecognitionError& re ) { success = false; }

	release();

	return success;
}

DeclarationNode* Parser::variableDecl(std::shared_ptr<string> struct_name)
{
	auto type_info = typeInfo();
	string var_name  = id();

	vector<ExprNode*> constructor_call_params;

	if ( getTokenType(1) == TokenType::LPAREN )
		constructor_call_params = call_params_list();
	else if ( getTokenType(1) == TokenType::ASSIGN )
	{
		match(TokenType::ASSIGN);
		constructor_call_params = {expression()};
	}

	return new VariableDeclarationNode(var_name, type_info, struct_name != nullptr, constructor_call_params);
}

DeclarationNode* Parser::structDecl()
{
	match(TokenType::STRUCT);

	string struct_name = id();

	vector<AST*> struct_in;
	match(TokenType::LBRACE);

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

DeclarationNode* Parser::templateStructDecl()
{
	match(TokenType::TEMPLATE);
	match(TokenType::LESS);

	vector< pair<string, TypeInfo> > template_params;

	if ( getTokenType(1) != TokenType::GREATER )
	{
		auto type_info = typeInfo();
		auto name = id();

		template_params.push_back({name, type_info});
		while ( getTokenType(1) == TokenType::COMMA )
		{
			match(TokenType::COMMA);

			type_info = typeInfo();
			name = id();

			template_params.push_back({name, type_info});
		}
	}

	match(TokenType::GREATER);

	match(TokenType::STRUCT);

	string struct_name = id();

	vector<AST*> struct_in;    
	match(TokenType::LBRACE);

	while ( getTokenType(1) != TokenType::RBRACE )
	{
		while ( getTokenType(1) == TokenType::SEMICOLON )
			match(TokenType::SEMICOLON);

		if ( getTokenType(1) != TokenType::RBRACE )
			struct_in.push_back(declaration(std::make_shared<string>(string(struct_name))));
	}

	match(TokenType::RBRACE);

	return new TemplateStructDeclarationNode(struct_name, struct_in, template_params);
}

DeclarationNode* Parser::functionDecl(std::shared_ptr<string> struct_name)
{
	match(TokenType::DEF);

	bool is_operator = getTokenType(1) == TokenType::OPERATOR; 

	string function_name = (is_operator ? operator_name() : id());

	vector< pair<string, TypeInfo> > params;

	match(TokenType::LPAREN);

	if ( getTokenType(1) != TokenType::RPAREN )
	{
		auto type_info = typeInfo();
		auto name = id();

		params.push_back({name, type_info});

		while ( getTokenType(1) != TokenType::RPAREN )
		{
			match(TokenType::COMMA);	   

			type_info = typeInfo();
			name = id();
			params.push_back({name, type_info});
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
			return_type = typeInfo();
		}
		else
			return_type = TypeInfo("void", false, false);
	}
	else
		return_type = TypeInfo(*struct_name, true, false);

	AST *statements = block();

	return new FunctionDeclarationNode(function_name, params, return_type, statements, {struct_name != nullptr, is_constructor, is_operator});
}

string Parser::id()
{
	string name = getToken(1).text;
	match(TokenType::ID);
	return name;
}

string Parser::operator_name()
{
	match(TokenType::OPERATOR);

	switch ( getTokenType(1) )
	{
	case TokenType::PLUS : match(TokenType::PLUS);  return "operator+";
	case TokenType::MINUS: match(TokenType::MINUS); return "operator-";
	case TokenType::MUL  : match(TokenType::MUL);   return "operator*";
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
	default:
		throw SemanticError("No such operator " + getToken(1).text);
	}
}

ExprNode* Parser::literal()
{
	if ( getTokenType(1) == TokenType::STRING )
		return get_string();
	else
		return number();
}

ExprNode* Parser::variable() { return new VariableNode(id()); }

ExprNode* Parser::get_string()
{
	string str = getToken(1).text;
	match(TokenType::STRING);
	return new StringNode(str);
}

ExprNode* Parser::number()
{
	string num = getToken(1).text;
	match(TokenType::NUMBER);
	return new NumberNode(num);
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
	else
		return variable();        
}

ExprNode* Parser::unary_right()
{
	ExprNode *res = primary();

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

ExprNode* Parser::unary_left()
{
	UnaryOp op;

	switch ( getTokenType(1) )
	{
	case TokenType::PLUS: op = UnaryOp::PLUS; break;
	case TokenType::MINUS: op = UnaryOp::MINUS; break;
	case TokenType::NOT: op = UnaryOp::NOT; break;
	default: return unary_right();
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
		BinaryOp op;

		switch ( getTokenType(1) )
		{
		case TokenType::PLUS : op = BinaryOp::PLUS; break;
		case TokenType::MINUS: op = BinaryOp::MINUS; break;
		default: throw;
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
		case TokenType::EQUALS : op = BinaryOp::EQUALS; break;
		case TokenType::NEQUALS: op = BinaryOp::NEQUALS; break;
		default: throw;
		}
		match(getTokenType(1));
		res = new BinaryOperatorNode(res, term(), op);
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
		case TokenType::AND : op = BinaryOp::AND; break;
		case TokenType::OR: op = BinaryOp::OR; break;
		default: throw;
		}
		match(getTokenType(1));
		res = new BinaryOperatorNode(res, term(), op);
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

	stats_true = statement();

	if ( getTokenType(1) == TokenType::ELSE )
	{
		match(TokenType::ELSE);
		stats_false = statement();
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

	AST *stats = statement();

	return new WhileNode(cond, stats);
}

bool Parser::tryAssignment()
{
	bool success = true;

	mark();

	try { assignment(); }
	catch ( RecognitionError& re ) { success = false; }

	release();

	return success;
}

AST* Parser::for_stat()
{
	match(TokenType::FOR);
	match(TokenType::LPAREN);

	AST *init;

	if ( getTokenType(1) == TokenType::SEMICOLON )
		init = new StatementNode({ }); 
	else if ( tryVarDecl() )	
		init = variableDecl();
	else if ( tryAssignment() )
		init = assignment();
	else
		throw RecognitionError("");

	match(TokenType::SEMICOLON);

	ExprNode *cond;

	if ( getTokenType(1) == TokenType::SEMICOLON )
		cond = new NumberNode("1");
	else
		cond = expression();

	match(TokenType::SEMICOLON);

	AST *step;

	if ( getTokenType(1) == TokenType::RPAREN )
		step = new StatementNode({ });
	else if ( tryAssignment() )
		step = assignment();
	else
		step = expression();

	match(TokenType::RPAREN);

	AST *stats = statement();
	
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

	string type_name = id();

	bool is_ref = false;

	vector<ExprNode*> template_params { };

	if ( getTokenType(1) == TokenType::LESS )
	{
		match(TokenType::LESS);

		if ( getTokenType(1) != TokenType::GREATER )
		{
			template_params.push_back(expression());
			while ( getTokenType(1) == TokenType::COMMA )
			{
				match(TokenType::COMMA);
				template_params.push_back(expression());
			}
		}

		match(TokenType::GREATER);
	}

	if ( getTokenType(1) == TokenType::REF )
	{
		is_ref = true;
		match(TokenType::REF);
	}

	return TypeInfo(type_name, is_ref, is_const, template_params);
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

	auto type = typeInfo();

	vector<ExprNode*> params = { };

	if ( getTokenType(1) == TokenType::LPAREN )
		params = call_params_list();

	return new NewExpressionNode(type, params);
}
