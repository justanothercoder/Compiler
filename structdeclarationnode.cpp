#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{
    definedSymbol = new StructSymbol("", nullptr);

    for ( auto i : inner )
	i->scope = (StructSymbol*)definedSymbol;
    
}

void StructDeclarationNode::define()
{
    *definedSymbol = StructSymbol(name, scope);

    for ( auto i : inner )
	i->define();
    
    scope->define(definedSymbol);
}

void StructDeclarationNode::gen()
{    
    for ( auto i : inner )
	i->gen();
}
