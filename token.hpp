#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>

using string = std::string;

enum class TokenType { EOF_TYPE,
	ID, NUMBER, LPAREN, RPAREN,
	LBRACE, RBRACE, COMMA, STRUCT,
	COLON, DEF, SEMICOLON, ASSIGN,
	PLUS, DIV, REF, RETURN, IF,
	ELSE, WHILE, DOT, MINUS, MUL,
	NEW, TEMPLATE, LESS, GREATER,
	LBRACKET, RBRACKET, OPERATOR,
	FOR, CONST, STRING, EQUALS,
	NEQUALS, NOT, AND, OR, IMPORT,
	MOD, VAR, UNSAFE
	};

string to_string(TokenType tt);

struct Token
{
    Token(TokenType type, string text, int line, int symbol);

    TokenType type;
    string text;

    int line, symbol;
};

#endif
