#include "ast.hpp"

AST::~AST() { }
 
Scope* AST::getScope() const { return GlobalHelper::getASTScope(this); }
void AST::setScope(Scope *sc) { return GlobalHelper::setASTScope(this, sc); }

void AST::define() { define(nullptr, { }); }
void AST::check() { check(nullptr, { }); }
void AST::gen() { gen(nullptr, { }); }

void AST::build_scope()
{
	std::vector<AST*> children = getChildren();

	for ( auto child : children )
	{
		child->setScope(this->getScope());
		child->build_scope();
	}
}
