#include "globalscope.hpp"

GlobalScope::GlobalScope()
{
    
}

Scope* GlobalScope::getEnclosingScope()
{
    return nullptr;
}

Symbol* GlobalScope::resolve(string name)
{
    auto it = table.find(name);
    if ( it == std::end(table) )
	return nullptr;
    return it->second;
}

void GlobalScope::define(Symbol *sym)
{
    if ( table.find(sym->getName()) == std::end(table) )
	table[sym->getName()] = sym;
}
