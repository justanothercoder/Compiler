#ifndef _ABSTRACTPARSER_HPP_
#define _ABSTRACTPARSER_HPP_

#include <vector>

#include "abstractlexer.hpp"

class AST;

using std::vector;

class AbstractParser
{
public:

    AbstractParser(AbstractLexer *input);

    virtual ~AbstractParser();
    
    virtual AST* parse() = 0;

protected:

    void consume();
    void match(TokenType token_type);

    void fill(int n);
    void seek(int i);
    void sync(int i);
    bool isSpeculating();

    Token getToken(int i);
    TokenType getTokenType(int i);
    
    int mark();
    void release();

protected:

    vector<Token> lookahead;
    vector<int> markers;

    AbstractLexer *input;
    int pos;    
        
};

#endif
