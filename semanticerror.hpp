#ifndef _SEMANTICERROR_HPP_
#define _SEMANTICERROR_HPP_

#include <stdexcept>
#include <string>

class SemanticError : public std::logic_error
{
public:

    SemanticError(std::string error);
};

#endif
