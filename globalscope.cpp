#include "globalscope.hpp"

GlobalScope::GlobalScope() : BaseScope() { } 

Scope* GlobalScope::getEnclosingScope() const { return nullptr; }
string GlobalScope::getScopeName() const { return ""; }

void GlobalScope::accept(ScopeVisitor *visitor) { visitor->visit(this); }

VarAllocator* GlobalScope::get_valloc() { return &valloc; }

TempAllocator& GlobalScope::getTempAlloc() { return temp_alloc; }
