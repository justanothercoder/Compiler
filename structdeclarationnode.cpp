#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{
    definedSymbol = nullptr;
}

StructDeclarationNode::~StructDeclarationNode() { delete definedSymbol; }

Symbol* StructDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void StructDeclarationNode::build_scope()
{
    definedSymbol = new StructSymbol(name, this->getScope());
    for ( auto i : inner )
    {
	i->setScope(definedSymbol);
	i->build_scope();
    }
}

void StructDeclarationNode::define()
{
    this->getScope()->define(definedSymbol);

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

void StructDeclarationNode::template_check() { std::for_each(std::begin(inner), std::end(inner), [] (DeclarationNode *decl) { decl->template_check(); }); }
