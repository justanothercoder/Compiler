#include "lexer.hpp"

Lexer::Lexer(string input) : AbstractLexer(input)
{
    
}

Token Lexer::getToken()
{
    while ( cur != (char)(-1) )
    {
	int l = line, s = symbol;
	
	if      ( cur == '(' ) { consume(); return Token(TokenType::LPAREN, "(", l, s); }
	else if ( cur == ')' ) { consume(); return Token(TokenType::RPAREN, ")", l, s); }
	else if ( cur == ',' ) { consume(); return Token(TokenType::COMMA, ",", l, s); }
	else if ( cur == '{' ) { consume(); return Token(TokenType::LBRACE, "{", l, s); }
	else if ( cur == '}' ) { consume(); return Token(TokenType::RBRACE, "}", l, s); }
	else if ( cur == ':' ) { consume(); return Token(TokenType::COLON, ":", l, s); }
	else if ( cur == ';' ) { consume(); return Token(TokenType::SEMICOLON, ";", l, s); }
	else if ( cur == '=' ) { consume(); return Token(TokenType::ASSIGN, "=", l, s); }
	else if ( cur == '+' ) { consume(); return Token(TokenType::PLUS, "+", l, s); }
	else if ( cur == '&' ) { consume(); return Token(TokenType::REF, "&", l, s); }
	else if ( cur == '/' )
	{
	    consume();
	    if ( cur == '/' )
	    {
		consume();
		while ( cur != '\n' && cur != (char)(-1) )
		    consume();
	    }
	    else if ( cur == '*' )
	    {
		while ( true )
		{
		    consume();
		    if ( cur == '*' )
		    {
			consume();
			if ( cur == '/' )
			{
			    consume();
			    break;
			}
		    }
		}
	    }
	    else
		return Token(TokenType::DIV, "/", l, s);
	}
	else if ( std::isspace(cur) )
	{
	    while ( std::isspace(cur) )
		consume();
	}
	else if ( std::isalpha(cur) )
	{
	    string buf = "";
	    while ( std::isalpha(cur) || std::isdigit(cur) || cur == '_' )
	    {
		buf += cur;
		consume();		
	    }

	    TokenType token_type;

	    if      ( buf == "struct" ) token_type = TokenType::STRUCT;
	    else if ( buf == "var"    ) token_type = TokenType::VAR;
	    else if ( buf == "def"    ) token_type = TokenType::DEF;
	    else if ( buf == "return" ) token_type = TokenType::RETURN;
	    else                        token_type = TokenType::ID;
	    
	    return Token(token_type, buf, l, s);
	}
	else if ( std::isdigit(cur) )
	{
	    string buf = "";
	    while ( std::isdigit(cur) )
	    {
		buf += cur;
		consume();		
	    }
	    return Token(TokenType::NUMBER, buf, l, s);	    
	}
	else
	    throw RecognitionError("unknown character");
    }

    return Token(TokenType::EOF_TYPE, "", line, symbol);
}
