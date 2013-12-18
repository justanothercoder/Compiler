#ifndef _RECOGNITIONERROR_HPP_
#define _RECOGNITIONERROR_HPP_

#include <exception>
#include <string>

using std::string;
using std::exception;

class RecognitionError : public exception
{
public:

    RecognitionError(string error);
    
    virtual ~RecognitionError() noexcept;
    virtual const char* what() const noexcept;
    
private:

    string error;
    
};

#endif
