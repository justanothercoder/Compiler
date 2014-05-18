#include "ast.hpp"

AST::~AST() { }
 
Scope* AST::getScope() const 
{
   	return scope; 
}

void AST::setScope(Scope *sc) 
{
   	scope = sc; 
}

void AST::build_scope()
{
	std::vector<AST*> children = getChildren();

	for ( auto child : children )
	{
		child->setScope(getScope());
		child->build_scope();
	}
}
