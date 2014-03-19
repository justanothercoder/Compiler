#include "ast.hpp"

AST::~AST() { }
 
Scope* AST::getScope() const { return GlobalHelper::getASTScope(const_cast<AST*>(this)); }
void AST::setScope(Scope *sc) { return GlobalHelper::setASTScope(this, sc); }

void AST::build_scope()
{
	std::vector<AST*> children = getChildren();

	for ( auto child : children )
	{
		child->setScope(getScope());
		child->build_scope();
	}
}
