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
	    return Token(TokenType::ID, buf, l, s);
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