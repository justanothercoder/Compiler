#include "globalscope.hpp"
#include "scopevisitor.hpp"
#include "functionscope.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "logger.hpp"

GlobalScope::GlobalScope() : BaseScope(), template_info(TemplateInfo())
{

}

Scope* GlobalScope::enclosingScope() const
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

const TemplateInfo& GlobalScope::templateInfo() const
{
    return template_info;
}

bool GlobalScope::isUnsafeBlock() const
{
    return false;
}
    
void GlobalScope::defineBuiltInFunction(std::string name, FunctionType type)
{
    std::string scope_name = getScopeName() + "_" + name;
    define(new FunctionSymbol(name, type, new FunctionScope(scope_name, this, false), FunctionTraits::simple())); 
}

void GlobalScope::defineBuiltInOperator(std::string name, FunctionType type)
{
    std::string scope_name = getScopeName() + "_" + GlobalConfig::getCodeOperatorName(name);
    define(new FunctionSymbol(name, type, new FunctionScope(scope_name, this, false), FunctionTraits::oper())); 
}
