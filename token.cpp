#include "token.hpp"

string to_string(TokenType tt)
{
    switch ( tt )
    {
    case TokenType::EOF_TYPE:  return "";
    case TokenType::ID:        return "id";
    case TokenType::STRUCT:    return "struct";
    case TokenType::NUMBER:    return "number";
    case TokenType::LPAREN:    return "(";
    case TokenType::RPAREN:    return ")";
    case TokenType::LBRACE:    return "{";
    case TokenType::RBRACE:    return "}";
    case TokenType::COMMA:     return ",";
    case TokenType::COLON:     return ":";
    case TokenType::SEMICOLON: return ";";
    case TokenType::VAR:       return "var";
    case TokenType::DEF:       return "def";
    }
}


Token::Token(TokenType type, string text, int line, int symbol) : type(type), text(text), line(line), symbol(symbol)
{
    
}
