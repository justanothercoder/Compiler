#include "lexer.hpp"

Lexer::Lexer(std::string input) : AbstractLexer(input) 
{

}

TokenType Lexer::getNameTokenType(const std::string& str)
{
    if      ( str == "struct"   ) return TokenType::STRUCT;
    else if ( str == "def"      ) return TokenType::DEF;
    else if ( str == "return"   ) return TokenType::RETURN;
    else if ( str == "if"       ) return TokenType::IF;
    else if ( str == "else"     ) return TokenType::ELSE;
    else if ( str == "while"    ) return TokenType::WHILE;
    else if ( str == "new"      ) return TokenType::NEW;
    else if ( str == "template" ) return TokenType::TEMPLATE;
    else if ( str == "operator" ) return TokenType::OPERATOR;
    else if ( str == "for"      ) return TokenType::FOR;
    else if ( str == "const"    ) return TokenType::CONST;
    else if ( str == "import"   ) return TokenType::IMPORT;
    else if ( str == "var"      ) return TokenType::VAR;
    else if ( str == "unsafe"   ) return TokenType::UNSAFE;
    else if ( str == "null"     ) return TokenType::NULLTOKEN;
    else if ( str == "extern"   ) return TokenType::EXTERN;
    else if ( str == "from"     ) return TokenType::FROM;
    else if ( str == "break"    ) return TokenType::BREAK;
    else if ( str == "lambda"   ) return TokenType::LAMBDA;
    else                          return TokenType::ID;
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
        else if ( cur == '+' ) { consume(); return Token(TokenType::PLUS, "+", l, s); }
        else if ( cur == '.' ) { consume(); return Token(TokenType::DOT, ".", l, s); }
        else if ( cur == '-' ) { consume(); return Token(TokenType::MINUS, "-", l, s); }
        else if ( cur == '*' ) { consume(); return Token(TokenType::MUL, "*", l, s); }
        else if ( cur == '<' ) { consume(); return Token(TokenType::LESS, "<", l, s); }
        else if ( cur == '>' ) { consume(); return Token(TokenType::GREATER, ">", l, s); }
        else if ( cur == '[' ) { consume(); return Token(TokenType::LBRACKET, "[", l, s); }
        else if ( cur == ']' ) { consume(); return Token(TokenType::RBRACKET, "]", l, s); }
        else if ( cur == '%' ) { consume(); return Token(TokenType::MOD, "%", l, s); }
        else if ( cur == '|' ) { consume(); match('|'); return Token(TokenType::OR, "||", l, s); }
        else if ( cur == '&' )
        {
            consume();
            if ( cur == '&' ) { consume(); return Token(TokenType::AND, "&&", l, s); }
            else                           return Token(TokenType::REF, "&", l, s);
        }
        else if ( cur == '!' )
        {
            consume();
            if ( cur == '=' ) { consume(); return Token(TokenType::NEQUALS, "!=", l, s); }
            else                           return Token(TokenType::NOT, "!", l, s);
        }
        else if ( cur == '=' )
        {
            consume();
            if ( cur == '=' ) { consume(); return Token(TokenType::EQUALS, "==", l, s); }
            else                           return Token(TokenType::ASSIGN, "=", l, s);
        }
        else if ( cur == '"' )
        {
            consume();

            std::string buf = "";

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
            std::string buf = "";
            while ( std::isalpha(cur) || std::isdigit(cur) || cur == '_' )
            {
                buf += cur;
                consume();
            }

            auto token_type = getNameTokenType(buf);
            return Token(token_type, buf, l, s);
        }
        else if ( std::isdigit(cur) )
        {
            std::string buf = "";
            while ( std::isdigit(cur) )
            {
                buf += cur;
                consume();
            }
            return Token(TokenType::NUMBER, buf, l, s);
        }
        else
            throw RecognitionError(std::string("unknown character '") + cur + "'", line, symbol);
    }

    return Token(TokenType::EOF_TYPE, "", line, symbol);
}
