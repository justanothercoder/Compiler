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
    auto scope_name = getScopeName() + "_" + name;
    auto scope = new FunctionScope(scope_name, this, false);
    define(std::make_shared<FunctionSymbol>(name, type, scope, FunctionTraits::simple())); 
}

void GlobalScope::defineBuiltInOperator(std::string name, FunctionType type)
{
    auto scope_name = getScopeName() + "_" + Comp::config().getCodeOperatorName(name);
    auto scope = new FunctionScope(scope_name, this, false);
    define(std::make_shared<FunctionSymbol>(name, type, scope, FunctionTraits::oper())); 
}
