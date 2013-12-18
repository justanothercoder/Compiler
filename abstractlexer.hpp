#ifndef _ABSTRACTLEXER_HPP_
#define _ABSTRACTLEXER_HPP_

#include <string>

using string = std::string;

class AbstractLexer
{
public:

    AbstractLexer(string input);
    
    virtual Token getToken() = 0;
    
protected:

    void consume();
    void match(char c);

protected:

    char cur;
    int pos;   
    string input;    
};

#endif
