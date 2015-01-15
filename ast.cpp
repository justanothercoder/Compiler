#include "ast.hpp"

AST::~AST() { } 

void AST::build_scope()
{
    auto children = getChildren();

    for ( const auto& child : children )
    {
        child -> scope = scope;
        child -> build_scope();
    }
}

ASTChildren AST::getChildren() const { return { }; }
