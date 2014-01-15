#ifndef _ABSTRACTLEXER_HPP_
#define _ABSTRACTLEXER_HPP_

#include <string>

#include "token.hpp"
#include "recognitionerror.hpp"

using string = std::string;

class AbstractLexer
{
public:
    
    AbstractLexer(string input);

    virtual ~AbstractLexer();
    
    virtual Token getToken() = 0;
    
protected:

    void consume();
    void match(char c);

protected:

    char cur;
    int pos;   
    string input;

    int line, symbol;
};

#endif
