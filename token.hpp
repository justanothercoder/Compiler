#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>

using string = std::string;

enum class TokenType { EOF_TYPE, ID, NUMBER, LPAREN, RPAREN, LBRACE, RBRACE, COMMA, STRUCT, VAR, COLON, DEF, SEMICOLON, ASSIGN, PLUS, DIV, REF };

string to_string(TokenType tt);

struct Token
{
    Token(TokenType type, string text, int line, int symbol);

    TokenType type;
    string text;

    int line, symbol;
};

#endif
