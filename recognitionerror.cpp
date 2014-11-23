#include "recognitionerror.hpp"

RecognitionError::RecognitionError(const std::string& message, int line, int symbol) : std::logic_error(message 
                                                                                                      + ":" 
                                                                                                      + std::to_string(line) 
                                                                                                      + ":" 
                                                                                                      + std::to_string(symbol))
{

}
