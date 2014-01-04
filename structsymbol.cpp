#include "structsymbol.hpp"

StructSymbol::StructSymbol(string name, Scope *enclosing_scope) : name(name), enclosing_scope(enclosing_scope)
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
	if ( getEnclosingScope() )
	    return getEnclosingScope()->resolve(name);
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

Type* StructSymbol::getTypeHint(ExprNode *expr)
{
    return type_hints[expr];
}

void StructSymbol::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}

int StructSymbol::getAddress(VariableSymbol *sym)
{
    auto it = addresses.find(sym);

    if ( it == std::end(addresses) )
    {
	if ( getEnclosingScope() == nullptr )
	    throw SemanticError("No such symbol " + sym->getName());

	return getEnclosingScope()->getAddress(sym) - (scope_address - getEnclosingScope()->getScopeAddress());
    }

    return it->second;    
}

int StructSymbol::getScopeAddress()
{
    return scope_address;
}

int StructSymbol::getScopeSize()
{
    return 0;
}

string StructSymbol::getScopeName()
{
    return scope_name;
}

Scope* StructSymbol::getScope()
{
    return symbol_scope;
}

void StructSymbol::setScope(Scope *scope)
{
    symbol_scope = scope;
}
