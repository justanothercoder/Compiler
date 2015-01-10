#include "structscope.hpp"
#include "scopevisitor.hpp"
#include "symbol.hpp"
#include "variablesymbol.hpp"

StructScope::StructScope(std::string name
                         , Scope *enclosing_scope
                         , const TemplateInfo& template_info) : scope_name(enclosing_scope -> getScopeName() + "_" + name)
                                                              , enclosing_scope(enclosing_scope)
                                                              , template_info(template_info)
{
    type_size = 0;
}

Scope* StructScope::enclosingScope() const { return enclosing_scope; }
std::string StructScope::getScopeName() const { return scope_name; }

const TemplateInfo& StructScope::templateInfo() const { return template_info; }    

void StructScope::accept(ScopeVisitor& visitor) { visitor.visit(this); }
