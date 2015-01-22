#include "structscope.hpp"
#include "scopevisitor.hpp"
#include "symbol.hpp"
#include "variablesymbol.hpp"

StructScope::StructScope(std::string name
                         , Scope *enclosing_scope) : scope_name(enclosing_scope -> getScopeName() + "_" + name)
                                                              , enclosing_scope(enclosing_scope)
{
    type_size = 0;
}

Scope* StructScope::enclosingScope() const { return enclosing_scope; }
std::string StructScope::getScopeName() const { return scope_name; }

void StructScope::accept(ScopeVisitor& visitor) { visitor.visit(this); }
