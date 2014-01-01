#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{

}

void StructDeclarationNode::build_scope()
{
    definedSymbol = new StructSymbol(name, scope);
    for ( auto i : inner )
    {
	i->scope = static_cast<StructSymbol*>(definedSymbol);
	i->build_scope();
    }
}

void StructDeclarationNode::define()
{
    scope->define(definedSymbol);

    for ( auto i : inner )
	i->define();    
}

void StructDeclarationNode::gen()
{    
    for ( auto i : inner )
	i->gen();
}

