#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{

}

StructDeclarationNode::~StructDeclarationNode() { delete definedSymbol; }

Symbol* StructDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void StructDeclarationNode::build_scope()
{
    definedSymbol = new StructSymbol(name, scope);
    for ( auto i : inner )
    {
	i->scope = definedSymbol;
	i->build_scope();
    }
}

void StructDeclarationNode::define()
{
    scope->define(definedSymbol);

    for ( auto i : inner )
	i->define();    
}

void StructDeclarationNode::check()
{
    for ( auto i : inner )
	i->check();
}

void StructDeclarationNode::gen()
{    
    for ( auto i : inner )
	i->gen();
}

