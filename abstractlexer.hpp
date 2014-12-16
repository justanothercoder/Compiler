#ifndef _ABSTRACTLEXER_HPP_
#define _ABSTRACTLEXER_HPP_

#include <string>

#include "token.hpp"
#include "recognitionerror.hpp"

class AbstractLexer
{
public:

    AbstractLexer(std::string input);
    virtual ~AbstractLexer();

    virtual Token getToken() = 0;

protected:

    void consume();
    void match(char c);

protected:

    char cur;
    int pos;
    std::string input;

    int line, symbol;
};

#endif
