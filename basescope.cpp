#include "basescope.hpp"

BaseScope::BaseScope()
{
    scope_size = 0;
}

Symbol* BaseScope::resolve(string name) const
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
    
Type* BaseScope::getTypeHint(ExprNode *expr) const
{
    auto it = type_hints.find(expr);

    if ( it == std::end(type_hints) )
	return nullptr;    

    return it->second;
}

void BaseScope::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}
    
int BaseScope::getAddress(VariableSymbol *sym) const
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

int BaseScope::getScopeSize() const
{
    return scope_size;
}

int BaseScope::getScopeAddress() const
{
    return scope_address;
}
