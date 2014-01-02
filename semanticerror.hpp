#ifndef _SEMANTICERROR_HPP_
#define _SEMANTICERROR_HPP_

#include <exception>
#include <string>

using std::string;
using std::exception;

class SemanticError : public exception
{
public:

    SemanticError(string error);
    
    virtual ~SemanticError() noexcept;
    virtual const char* what() const noexcept;
    
private:

    string error;
    
};

#endif
