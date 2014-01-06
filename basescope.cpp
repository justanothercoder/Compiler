#include "basescope.hpp"

BaseScope::BaseScope()
{
    scope_size = 0;
}

Symbol* BaseScope::resolve(string name)
{
    auto it = table.find(name);
    if ( it == std::end(table) )
    {
	if ( getEnclosingScope() )
	    return getEnclosingScope()->resolve(name);
	return nullptr;
    }
    return it->second;    
}
    
Type* BaseScope::getTypeHint(ExprNode *expr)
{
    return type_hints[expr];
}

void BaseScope::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}
    
int BaseScope::getAddress(VariableSymbol *sym)
{
    auto it = addresses.find(sym);

    if ( it == std::end(addresses) )
    {
	if ( getEnclosingScope() == nullptr )
	    throw SemanticError("No such symbol " + ((Symbol*)sym)->getName());

	return getEnclosingScope()->getAddress(sym) - (scope_address - getEnclosingScope()->getScopeAddress());
    }

    return it->second;
}

int BaseScope::getScopeSize()
{
    return scope_size;
}

int BaseScope::getScopeAddress()
{
    return scope_address;
}
