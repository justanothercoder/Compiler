#include "semanticerror.hpp"

SemanticError::SemanticError(string error) : error(error)
{
    
}

SemanticError::~SemanticError() noexcept
{
    
}

const char* SemanticError::what() const noexcept
{
    return ("Error: " + error).c_str();
}
