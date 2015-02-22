#include "globalscope.hpp"
#include "functionscope.hpp"
#include "symbolfactory.hpp"
#include "globalconfig.hpp"
#include "logger.hpp"
#include "comp.hpp"

GlobalScope::GlobalScope() : BaseScope() { } 

Scope* GlobalScope::enclosingScope() const { return nullptr; }
std::string GlobalScope::getScopeName() const { return ""; }

bool GlobalScope::isUnsafeBlock() const { return false; }

void GlobalScope::defineBuiltInFunction(std::string name, FunctionType type)
{
    SymbolFactory factory;

    auto scope_name = getScopeName() + "_" + name;
    auto scope = new FunctionScope(scope_name, this, false);
    define(factory.makeFunction(name, type, FunctionTraits::simple(), false, scope));
}

void GlobalScope::defineBuiltInOperator(std::string name, FunctionType type)
{
    SymbolFactory factory;

    auto scope_name = getScopeName() + "_" + Comp::config().getCodeOperatorName(name);
    auto scope = new FunctionScope(scope_name, this, false);

    define(factory.makeFunction(name, type, FunctionTraits::oper(), false, scope));
}
