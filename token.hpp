#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>

using string = std::string;

enum class TokenType { ID, NUMBER, LPAREN, RPAREN, LBRACE, RBRACE, COMMA };

struct Token
{
    Token(TokenType type, string text, int line, int symbol);

    TokenType type;
    string text;

    int line, symbol;
};

#endif
