#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <string>
#include <cctype>

#include "abstractlexer.hpp"

class Lexer : public AbstractLexer
{
public:

    Lexer(std::string input);
    
    Token getToken() override;
    
};

#endif
