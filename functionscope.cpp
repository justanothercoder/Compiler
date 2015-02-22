#include "functionscope.hpp"
#include "globalconfig.hpp"

FunctionScope::FunctionScope(std::string scope_name
                           , Scope *enclosing_scope
                           , bool is_unsafe) : scope_name     (scope_name)
                                             , enclosing_scope(enclosing_scope)
                                             , is_unsafe      (is_unsafe)
{

}

Scope* FunctionScope::enclosingScope() const { return enclosing_scope; }
std::string FunctionScope::getScopeName() const { return scope_name; }

bool FunctionScope::isUnsafeBlock() const { return is_unsafe; }
