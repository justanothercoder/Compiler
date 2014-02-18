#include "globalscope.hpp"

GlobalScope::GlobalScope() : BaseScope() { } 

Scope* GlobalScope::getEnclosingScope() const { return nullptr; }
string GlobalScope::getScopeName() const { return ""; }

void GlobalScope::accept(ScopeVisitor *visitor) { visitor->visit(this); }

int GlobalScope::getFreeAddress() const { return getScopeSize() + GlobalConfig::int_size; }
