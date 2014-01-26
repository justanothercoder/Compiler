#include "token.hpp"

string to_string(TokenType tt)
{
    switch ( tt )
    {
    case TokenType::EOF_TYPE:  return "";
    case TokenType::ID:        return "id";
    case TokenType::STRUCT:    return "struct";
    case TokenType::NUMBER:    return "number";
    case TokenType::RETURN:    return "return";
    case TokenType::IF:        return "if";
    case TokenType::ELSE:      return "else";
    case TokenType::WHILE:     return "while";
    case TokenType::LPAREN:    return "(";
    case TokenType::RPAREN:    return ")";
    case TokenType::LBRACE:    return "{";
    case TokenType::RBRACE:    return "}";
    case TokenType::COMMA:     return ",";
    case TokenType::COLON:     return ":";
    case TokenType::SEMICOLON: return ";";
    case TokenType::ASSIGN:    return "=";
    case TokenType::PLUS:      return "+";
    case TokenType::MINUS:     return "-";
    case TokenType::MUL:       return "*";
    case TokenType::DIV:       return "/";
    case TokenType::REF:       return "&";
    case TokenType::DOT:       return ".";
    case TokenType::VAR:       return "var";
    case TokenType::DEF:       return "def";
    case TokenType::NEW:       return "new";
    }
}


Token::Token(TokenType type, string text, int line, int symbol) : type(type), text(text), line(line), symbol(symbol)
{
    
}
