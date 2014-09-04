#include "scope.hpp"
#include "symbol.hpp"

Scope::~Scope() 
{

}

bool Scope::isUnsafeBlock() const
{
	return false;
}

void Scope::define(Symbol *sym)
{
	accept(sym -> getScopeVisitor());
}
