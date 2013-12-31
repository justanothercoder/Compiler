#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{
    StructSymbol *ss = new StructSymbol("", nullptr);

    definedSymbol = ss;
    scope = ss;

    for ( auto i : inner )
	i->scope = static_cast<StructSymbol*>(definedSymbol);
    
}

StructDeclarationNode::~StructDeclarationNode()
{
    delete definedSymbol;
    
    scope = nullptr;
    definedSymbol = nullptr;
}

void StructDeclarationNode::define()
{
    definedSymbol->setName(name);
    static_cast<StructSymbol*>(definedSymbol)->setEnclosingScope(scope);

    for ( auto i : inner )
	i->define();
    
    scope->define(definedSymbol);
}

void StructDeclarationNode::gen()
{    
    for ( auto i : inner )
	i->gen();
}

