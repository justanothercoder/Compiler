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
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	if ( table[sym->getName()] == nullptr )
	    table[sym->getName()] = new VariableSymbol(sym->getName(), new OverloadedFunctionType({ }));

	OverloadedFunctionType *ot = static_cast<OverloadedFunctionType*>(static_cast<VariableSymbol*>(table[sym->getName()])->getType());
	
	FunctionType *ft = static_cast<FunctionType*>(static_cast<FunctionSymbol*>(sym)->getType());
	ot->overloads.insert(ft);
	ot->symbols[ft] = static_cast<FunctionSymbol*>(sym);
    }
    else
	table[sym->getName()] = sym;
}
    
Type* GlobalScope::getTypeHint(ExprNode *expr)
{
    return type_hints[expr];
}

void GlobalScope::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}
    
