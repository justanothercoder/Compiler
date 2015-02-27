#include "unsafescope.hpp"

UnsafeScope::UnsafeScope(Scope *enclosing_scope) : LocalScope(enclosing_scope) { }

bool UnsafeScope::isUnsafeBlock() const { return true; }
