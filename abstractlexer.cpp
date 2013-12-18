#include "abstractlexer.hpp"

AbstractLexer::AbstractLexer(string input) : input(input)
{
    pos = 0;
    consume();
}

void AbstractLexer::consume()
{
    if ( pos >= input.length() )
	cur = EOF;
    else
    {
	cur = input[pos];
	++pos;
    }
}

void AbstractLexer::match(char c)
{
    if ( cur == c )
	consume();
    else
	throw RecognitionError(string("expected ") + c + string(", but found ") + cur);
}
