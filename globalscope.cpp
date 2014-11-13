#include "globalscope.hpp"
#include "scopevisitor.hpp"
#include "functionscope.hpp"
#include "functionsymbol.hpp"

GlobalScope::GlobalScope() : BaseScope(), template_info(TemplateInfo())
{

}

Scope* GlobalScope::getEnclosingScope() const
{
    return nullptr;
}

std::string GlobalScope::getScopeName() const
{
    return "";
}

void GlobalScope::accept(ScopeVisitor& visitor)
{
    visitor.visit(this);
}

VarAllocator& GlobalScope::getVarAlloc() const
{
    return var_alloc;
}

TempAllocator& GlobalScope::getTempAlloc() const
{
    return temp_alloc;
}

const TemplateInfo& GlobalScope::getTemplateInfo() const
{
    return template_info;
}

bool GlobalScope::isUnsafeBlock() const
{
    return false;
}
    
void GlobalScope::defineBuiltInFunction(std::string name, const FunctionType *type)
{
    define(new FunctionSymbol(name, type, new FunctionScope(getScopeName() + "_" + name, this, false), {false, false, false})); 
}
