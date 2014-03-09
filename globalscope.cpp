#include "globalscope.hpp"

GlobalScope::GlobalScope() : BaseScope() { } 

Scope* GlobalScope::getEnclosingScope() const { return nullptr; }
string GlobalScope::getScopeName() const { return ""; }

void GlobalScope::accept(ScopeVisitor *visitor) { visitor->visit(this); }

VarAllocator* GlobalScope::get_valloc() const { return &valloc; }
