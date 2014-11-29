#include "abstractlexer.hpp"

AbstractLexer::AbstractLexer(string input) : input(input)
{
    pos = 0;
    cur = '\0';

    consume();

    line = 1;
    symbol = 1;
}

AbstractLexer::~AbstractLexer() { }

void AbstractLexer::consume()
{
    if ( pos >= (int)input.length() )
        cur = EOF;
    else
    {
        if ( cur == '\n' )
        {
            ++line;
            symbol = 0;
        }

        ++symbol;

        cur = input[pos];
        ++pos;
    }
}

void AbstractLexer::match(char c)
{
    if ( cur == c )
        consume();
    else
        throw RecognitionError(std::string("expected ") + c + std::string(", but found ") + cur, line, symbol);
}
