#include "globalscope.hpp"
#include "scopevisitor.hpp"
#include "functionscope.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "logger.hpp"
#include "comp.hpp"

GlobalScope::GlobalScope() : BaseScope(), template_info(TemplateInfo()) { } 

Scope* GlobalScope::enclosingScope() const { return nullptr; }
std::string GlobalScope::getScopeName() const { return ""; }

bool GlobalScope::isUnsafeBlock() const { return false; }
const TemplateInfo& GlobalScope::templateInfo() const { return template_info; }

void GlobalScope::accept(ScopeVisitor& visitor) { visitor.visit(this); }

void GlobalScope::defineBuiltInFunction(std::string name, FunctionType type)
{
    std::string scope_name = getScopeName() + "_" + name;
    define(new FunctionSymbol(name, type, new FunctionScope(scope_name, this, false), FunctionTraits::simple())); 
}

void GlobalScope::defineBuiltInOperator(std::string name, FunctionType type)
{
    std::string scope_name = getScopeName() + "_" + Comp::config().getCodeOperatorName(name);
    define(new FunctionSymbol(name, type, new FunctionScope(scope_name, this, false), FunctionTraits::oper())); 
}
