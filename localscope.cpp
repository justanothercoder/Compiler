#include "localscope.hpp"

LocalScope::LocalScope(Scope *enclosing_scope) : enclosing_scope(enclosing_scope) { }

Scope* LocalScope::getEnclosingScope() const { return enclosing_scope; }
string LocalScope::getScopeName() const { return ""; }

void LocalScope::accept(ScopeVisitor *visitor) { visitor -> visit(this); }

VarAllocator& LocalScope::getVarAlloc() { return getEnclosingScope() -> getVarAlloc(); }

TempAllocator& LocalScope::getTempAlloc() { return getEnclosingScope() -> getTempAlloc(); }
