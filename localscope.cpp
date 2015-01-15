#include "localscope.hpp"
#include "scopevisitor.hpp"

LocalScope::LocalScope(Scope* enclosing_scope) : enclosing_scope(enclosing_scope) { }

Scope* LocalScope::enclosingScope() const { return enclosing_scope; }
std::string LocalScope::getScopeName() const { return ""; }

void LocalScope::accept(ScopeVisitor& visitor) { visitor.visit(this); }

const TemplateInfo& LocalScope::templateInfo() const { return enclosingScope() -> templateInfo(); }
bool LocalScope::isUnsafeBlock() const { return enclosingScope() -> isUnsafeBlock(); }
