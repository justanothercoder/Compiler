#include "token.hpp"

Token::Token(TokenType type, std::string text, int line, int symbol) : type(type), text(text), line(line), symbol(symbol)
{

}

std::string to_string(TokenType tt)
{
    switch ( tt )
    {
    case TokenType::EOF_TYPE : return "";
    case TokenType::ID       : return "id";
    case TokenType::STRUCT   : return "struct";
    case TokenType::NUMBER   : return "number";
    case TokenType::RETURN   : return "return";
    case TokenType::IF       : return "if";
    case TokenType::ELSE     : return "else";
    case TokenType::WHILE    : return "while";
    case TokenType::DEF      : return "def";
    case TokenType::NEW      : return "new";
    case TokenType::TEMPLATE : return "template";
    case TokenType::OPERATOR : return "operator";
    case TokenType::FOR      : return "for";
    case TokenType::CONST    : return "const";
    case TokenType::STRING   : return "string";
    case TokenType::IMPORT   : return "import";
    case TokenType::VAR      : return "var";
    case TokenType::UNSAFE   : return "unsafe";
    case TokenType::NULLTOKEN: return "null";
    case TokenType::LPAREN   : return "(";
    case TokenType::RPAREN   : return ")";
    case TokenType::LBRACE   : return "{";
    case TokenType::RBRACE   : return "}";
    case TokenType::LBRACKET : return "[";
    case TokenType::RBRACKET : return "]";
    case TokenType::COMMA    : return ",";
    case TokenType::COLON    : return ":";
    case TokenType::SEMICOLON: return ";";
    case TokenType::ASSIGN   : return "=";
    case TokenType::PLUS     : return "+";
    case TokenType::MINUS    : return "-";
    case TokenType::MUL      : return "*";
    case TokenType::DIV      : return "/";
    case TokenType::REF      : return "&";
    case TokenType::DOT      : return ".";
    case TokenType::LESS     : return "<";
    case TokenType::GREATER  : return ">";
    case TokenType::NOT      : return "!";
    case TokenType::MOD      : return "%";
    case TokenType::EQUALS   : return "==";
    case TokenType::NEQUALS  : return "!=";
    case TokenType::AND      : return "&&";
    case TokenType::OR       : return "||";
    case TokenType::EXTERN   : return "extern";                       
    case TokenType::FROM     : return "from";
    case TokenType::BREAK    : return "break";
    case TokenType::LAMBDA   : return "lambda";
    }
}
