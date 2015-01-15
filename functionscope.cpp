#include "functionscope.hpp"
#include "scopevisitor.hpp"
#include "globalconfig.hpp"
#include "functionsymbol.hpp"

FunctionScope::FunctionScope(std::string scope_name
                           , Scope *enclosing_scope
                           , bool is_unsafe) : func           (nullptr)                                             
                                             , template_info  (enclosing_scope -> templateInfo())
                                             , scope_name     (scope_name)
                                             , enclosing_scope(enclosing_scope)
                                             , is_unsafe      (is_unsafe)
{

}

Scope* FunctionScope::enclosingScope() const { return enclosing_scope; }
std::string FunctionScope::getScopeName() const { return scope_name; }

void FunctionScope::accept(ScopeVisitor& visitor) { visitor.visit(this); }

const TemplateInfo& FunctionScope::templateInfo() const { return template_info; }

bool FunctionScope::isUnsafeBlock() const { return is_unsafe; }
