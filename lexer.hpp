#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <string>
#include <cctype>

#include "abstractlexer.hpp"

using std::string;

class Lexer : public AbstractLexer
{
public:

    Lexer(string input);
    
    virtual Token getToken();
    
};

#endif
