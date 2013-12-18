#include "declarationnode.hpp"

DeclarationNode::~DeclarationNode()
{
    delete definedSymbol;
}

void DeclarationNode::process()
{
    define();
    gen();
}
