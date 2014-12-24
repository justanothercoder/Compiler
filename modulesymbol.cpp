#include "modulesymbol.hpp"
#include "scopevisitor.hpp"
#include "templateinfo.hpp"

ModuleSymbol::ModuleSymbol(std::string name, Scope* enclosing_scope) : name(name), enclosing_scope(enclosing_scope)
{

}

std::string ModuleSymbol::getName() const { return name; }

SymbolType ModuleSymbol::getSymbolType() const { return SymbolType::MODULE; }
    
Scope* ModuleSymbol::enclosingScope() const { return enclosing_scope; }
std::string ModuleSymbol::getScopeName() const { return getName(); }

void ModuleSymbol::accept(ScopeVisitor& visitor) { visitor.visit(this); }

const TemplateInfo& ModuleSymbol::templateInfo() const { return *(new TemplateInfo()); }
