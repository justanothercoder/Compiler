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

VarAllocator& GlobalScope::varAlloc() const
{
    return var_alloc;
}

TempAllocator& GlobalScope::tempAlloc() const
{
    return temp_alloc;
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
    define(new FunctionSymbol(name
                            , type
                            , new FunctionScope(getScopeName() + "_" + name
                                              , this
                                              , false
                                              , false)
                            , FunctionTraits::simple()
                            )
            ); 
}

void GlobalScope::defineBuiltInOperator(std::string name, FunctionType type)
{
    define(new FunctionSymbol(name
                            , type
                            , new FunctionScope(getScopeName() + "_" + GlobalConfig::getCodeOperatorName(name)
                                              , this
                                              , false
                                              , false)
                            , FunctionTraits::oper()
                            )
            ); 
}
