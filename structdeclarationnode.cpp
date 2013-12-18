#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{
    
}

void StructDeclarationNode::define()
{
    StructSymbol *struc = new StructSymbol(name, scope);

    for ( auto i : inner )
	i->define();
    
    scope->define(struc);
}

void StructDeclarationNode::gen()
{    
    for ( auto i : inner )
	i->gen();
}
