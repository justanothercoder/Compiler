#include "modulesymbol.hpp"

ModuleSymbol::ModuleSymbol(std::string name, Scope* enclosing_scope) : name(name), enclosing_scope(enclosing_scope) { }
std::string ModuleSymbol::getName() const { return name; }

Scope* ModuleSymbol::enclosingScope() const { return enclosing_scope; }
std::string ModuleSymbol::getScopeName() const { return getName(); }

bool ModuleSymbol::isModule() const { return true; }
