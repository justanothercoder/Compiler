#include "globalscope.hpp"

GlobalScope::GlobalScope() 
{
    scope_size = 0;
    scope_address = 0;
}

Scope* GlobalScope::getEnclosingScope() const { return nullptr; }
string GlobalScope::getScopeName() const { return ""; }

void GlobalScope::accept(ScopeVisitor *visitor) { visitor->visit(this); }

int GlobalScope::getFreeAddress() const { return getScopeSize() + GlobalConfig::int_size; }
