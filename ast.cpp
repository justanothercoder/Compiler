#include "ast.hpp"

AST::~AST()
{
    delete scope;
}
