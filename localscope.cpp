#include "localscope.hpp"

LocalScope::LocalScope(Scope *enclosing_scope) : enclosing_scope(enclosing_scope) { }

Scope* LocalScope::getEnclosingScope() const { return enclosing_scope; }
string LocalScope::getScopeName() const { return ""; }

void LocalScope::accept(ScopeVisitor *visitor) { visitor->visit(this); }

int LocalScope::getFreeAddress() const { return getEnclosingScope()->getScopeSize() + getScopeSize() + GlobalConfig::int_size; }

int LocalScope::getAddress(VariableSymbol *sym) const 
{
	Scope *sc = getEnclosingScope();

	while ( !dynamic_cast<FunctionSymbol*>(sc) && !dynamic_cast<GlobalScope*>(sc) )
		sc = sc->getEnclosingScope();

	return this->BaseScope::getAddress(sym) + getScopeAddress() - sc->getScopeAddress();
}

//void LocalScope::recalc_scope_address() { scope_address = enclosing_scope->getScopeAddress(); }

void LocalScope::recalc_scope_address() { scope_address = enclosing_scope->getScopeAddress() + getScopeSize(); }
