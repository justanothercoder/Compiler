#include "localscope.hpp"
#include "scopevisitor.hpp"

LocalScope::LocalScope(Scope *enclosing_scope) : enclosing_scope(enclosing_scope)
{

}

Scope* LocalScope::getEnclosingScope() const
{
    return enclosing_scope;
}

std::string LocalScope::getScopeName() const
{
    return "";
}

void LocalScope::accept(ScopeVisitor& visitor)
{
    visitor.visit(this);
}

VarAllocator& LocalScope::getVarAlloc() const
{
    return getEnclosingScope() -> getVarAlloc();
}

TempAllocator& LocalScope::getTempAlloc() const
{
    return getEnclosingScope() -> getTempAlloc();
}

const TemplateInfo& LocalScope::getTemplateInfo() const
{
    return getEnclosingScope() -> getTemplateInfo();
}

bool LocalScope::isUnsafeBlock() const
{
    return getEnclosingScope() -> isUnsafeBlock();
}
