#include "abstractparser.hpp"

AbstractParser::AbstractParser(AbstractLexer *input) : input(input)
{
    pos = -1;
    consume();
}

AbstractParser::~AbstractParser()
{
    
}

void AbstractParser::consume()
{
    ++pos;
    
    if ( pos == (int)lookahead.size() && !isSpeculating() )
    {
	pos = 0;
	lookahead.clear();
    }
    
    sync(1);
}

void AbstractParser::match(TokenType token_type)
{
    if ( getTokenType(1) == token_type )
	consume();
    else
	throw RecognitionError(to_string(token_type) + " was expected, but token '" + getToken(1).text + "' was found.");
}

Token AbstractParser::getToken(int i)
{
    sync(i);
    return lookahead[pos + i - 1];
}

TokenType AbstractParser::getTokenType(int i)
{
    return getToken(i).type;
}

void AbstractParser::sync(int i)
{
    if ( pos + i - 1 >= (int)lookahead.size() )
	fill((pos + i - 1) - (lookahead.size() - 1));
}

void AbstractParser::fill(int n)
{
    for ( int i = 0; i < n; ++i )
	lookahead.push_back(input->getToken());
}

int AbstractParser::mark()
{
    markers.push_back(pos);
    return pos;
}

void AbstractParser::release()
{
    int marker = markers.back();
    markers.pop_back();
    seek(marker);
}

void AbstractParser::seek(int i)
{
    pos = i;
}

bool AbstractParser::isSpeculating()
{
    return markers.size() > 0;
}
