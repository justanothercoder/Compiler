#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{
    definedSymbol = new StructSymbol("", scope);

    for ( auto i : inner )
	i->scope = static_cast<StructSymbol*>(definedSymbol);
    
}

void StructDeclarationNode::define()
{
    definedSymbol->setName(name);

    for ( auto i : inner )
	i->define();
    
    scope->define(definedSymbol);
}

void StructDeclarationNode::gen()
{    
    for ( auto i : inner )
	i->gen();
}

