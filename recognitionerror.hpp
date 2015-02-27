#ifndef _RECOGNITIONERROR_HPP_
#define _RECOGNITIONERROR_HPP_

#include <stdexcept>

class RecognitionError : public std::logic_error
{
public:
    RecognitionError(std::string message, int line, int symbol);
};

#endif
