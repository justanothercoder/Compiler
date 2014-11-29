#include "modulesymbol.hpp"
#include "varallocator.hpp"
#include "tempallocator.hpp"
#include "scopevisitor.hpp"
#include "templateinfo.hpp"

ModuleSymbol::ModuleSymbol(std::string name, Scope* enclosing_scope) : name(name), enclosing_scope(enclosing_scope)
{

}

std::string ModuleSymbol::getName() const
{
    return name;
}

SymbolType ModuleSymbol::getSymbolType() const
{
    return SymbolType::MODULE;
}
    
Scope* ModuleSymbol::getEnclosingScope() const 
{
    return enclosing_scope;
}

VarAllocator& ModuleSymbol::getVarAlloc() const 
{
    return *(new VarAllocator());
}

TempAllocator& ModuleSymbol::getTempAlloc() const 
{
    return *(new TempAllocator());
}

std::string ModuleSymbol::getScopeName() const
{
    return getName();
}

void ModuleSymbol::accept(ScopeVisitor& visitor) 
{
    visitor.visit(this);
}

const TemplateInfo& ModuleSymbol::getTemplateInfo() const 
{
    return *(new TemplateInfo());
}
