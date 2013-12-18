#include "recognitionerror.hpp"

RecognitionError::RecognitionError(string error) : error(error)
{
    
}

RecognitionError::~RecognitionError() noexcept
{
    
}

const char* RecognitionError::what() const noexcept
{
    return "Error: " + error;
}
