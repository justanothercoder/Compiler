#include "localscope.hpp"

LocalScope::LocalScope(Scope* enclosing_scope) : enclosing_scope(enclosing_scope) { }

Scope* LocalScope::enclosingScope() const { return enclosing_scope; }
std::string LocalScope::getScopeName() const { return ""; }

bool LocalScope::isUnsafeBlock() const { return enclosingScope() -> isUnsafeBlock(); }
