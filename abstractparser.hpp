#ifndef _ABSTRACTPARSER_HPP_
#define _ABSTRACTPARSER_HPP_

#include <vector>
#include <memory>
#include "abstractlexer.hpp"

class AST;

class AbstractParser
{
public:

    AbstractParser(std::unique_ptr<AbstractLexer> input);
    virtual ~AbstractParser();

    virtual std::unique_ptr<AST> parse() = 0;

protected:

    void consume();
    void match(TokenType token_type);

    void fill(size_t n);
    void seek(int i);
    void sync(int i);
    bool isSpeculating();

    Token getToken(int i);
    TokenType getTokenType(int i);

    int mark();
    void release();

protected:

    std::vector<Token> lookahead;
    std::vector<int> markers;

    std::unique_ptr<AbstractLexer> input;
    int pos;

};

#endif
