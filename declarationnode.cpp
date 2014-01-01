#include "declarationnode.hpp"

DeclarationNode::~DeclarationNode()
{
    delete definedSymbol;
}
