#include "semanticerror.hpp"

SemanticError::SemanticError(std::string error) : std::logic_error("Error: " + error)
{
    
}
