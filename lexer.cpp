#include "lexer.hpp"

Lexer::Lexer(string input) : AbstractLexer(input) { }

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
		else if ( cur == '+' ) { consume(); return Token(TokenType::PLUS, "+", l, s); }
		else if ( cur == '.' ) { consume(); return Token(TokenType::DOT, ".", l, s); }
		else if ( cur == '-' ) { consume(); return Token(TokenType::MINUS, "-", l, s); }
		else if ( cur == '*' ) { consume(); return Token(TokenType::MUL, "*", l, s); }
		else if ( cur == '<' ) { consume(); return Token(TokenType::LESS, "<", l, s); }
		else if ( cur == '>' ) { consume(); return Token(TokenType::GREATER, ">", l, s); }
		else if ( cur == '[' ) { consume(); return Token(TokenType::LBRACKET, "[", l, s); }
		else if ( cur == ']' ) { consume(); return Token(TokenType::RBRACKET, "]", l, s); }
		else if ( cur == '|' ) 
		{
			consume();
			match('|');			
			return Token(TokenType::OR, "||", l, s);
		}
		else if ( cur == '&' ) 
		{ 
			consume(); 
			if ( cur == '&' )
			{
				consume();
				return Token(TokenType::AND, "&&", l, s);
			}
			else
				return Token(TokenType::REF, "&", l, s); 
		}
		else if ( cur == '!' )
		{
			consume();
			if ( cur == '=' )
			{
				consume();
				return Token(TokenType::NEQUALS, "!=", l, s);
			}
			else
				return Token(TokenType::NOT, "!", l, s);
		}
		else if ( cur == '=' ) 
		{ 
			consume();
			if ( cur == '=' )
			{
				consume();
				return Token(TokenType::EQUALS, "==", l, s);
			}
			else
				return Token(TokenType::ASSIGN, "=", l, s); 
		}
		else if ( cur == '"' ) 
		{
			consume();

			string buf = "";			

			while ( cur != '"' )
			{
				buf += cur;
				consume();				
			}

			consume();

			return Token(TokenType::STRING, buf, l, s);
		}
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
		else if ( std::isalpha(cur) || cur == '_' )
		{
			string buf = "";
			while ( std::isalpha(cur) || std::isdigit(cur) || cur == '_' )
			{
				buf += cur;
				consume();		
			}

			TokenType token_type;

			if      ( buf == "struct"   ) token_type = TokenType::STRUCT;
			else if ( buf == "def"      ) token_type = TokenType::DEF;
			else if ( buf == "return"   ) token_type = TokenType::RETURN;
			else if ( buf == "if"       ) token_type = TokenType::IF;
			else if ( buf == "else"     ) token_type = TokenType::ELSE;
			else if ( buf == "while"    ) token_type = TokenType::WHILE;
			else if ( buf == "new"      ) token_type = TokenType::NEW;
			else if ( buf == "template" ) token_type = TokenType::TEMPLATE;
			else if ( buf == "operator" ) token_type = TokenType::OPERATOR;
			else if ( buf == "for"      ) token_type = TokenType::FOR;
			else if ( buf == "const"    ) token_type = TokenType::CONST;
			else if ( buf == "import"   ) token_type = TokenType::IMPORT;
			else                          token_type = TokenType::ID;

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
			throw RecognitionError(string("unknown character '") + cur + "'.");
	}

	return Token(TokenType::EOF_TYPE, "", line, symbol);
}
