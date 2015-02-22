#include "structscope.hpp"

StructScope::StructScope(std::string name
                         , Scope *enclosing_scope) : scope_name(enclosing_scope -> getScopeName() + "_" + name)
                                                              , enclosing_scope(enclosing_scope), name(name)
{
    type_size = 0;
}

Scope* StructScope::enclosingScope() const { return enclosing_scope; }
std::string StructScope::getScopeName() const { return scope_name; }

//bool StructScope::isUnsafeBlock() const { return is_unsafe; }
bool StructScope::isUnsafeBlock() const { return false; }
