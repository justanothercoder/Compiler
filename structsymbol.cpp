#include "structsymbol.hpp"

StructSymbol::StructSymbol(string name, Scope *enclosing_scope) : Symbol(name), enclosing_scope(enclosing_scope)
{
    type_size = 0;
}

Scope* StructSymbol::getEnclosingScope()
{
    return enclosing_scope;    
}

Symbol* StructSymbol::resolve(string name)
{
    auto it = members.find(name);
    if ( it == std::end(members) )
    {
	if ( enclosing_scope )
	    return enclosing_scope->resolve(name);
	return nullptr;
    }
    return it->second;
}

void StructSymbol::define(Symbol *sym)
{
    if ( members.find(name) == std::end(members) )
    {
	members[sym->getName()] = sym;

	if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
	    type_size += static_cast<VariableSymbol*>(sym)->getType()->getSize();
    }
}
int StructSymbol::getSize()
{
    return type_size;
}

string StructSymbol::getName()
{
    return name;
}
