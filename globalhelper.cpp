#include "globalhelper.hpp"

map<ExprNode*, Type*> GlobalHelper::type_hints = map<ExprNode*, Type*>();
map<Symbol*, Scope*> GlobalHelper::symbol_scopes = map<Symbol*, Scope*>();

map<Symbol*, int> GlobalHelper::definition_time = map<Symbol*, int>();

Type* GlobalHelper::getTypeHint(ExprNode *expr)
{
    auto it = type_hints.find(expr);

    if ( it == std::end(type_hints) )	
	return nullptr;

    return it->second;
}

void GlobalHelper::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}

Scope* GlobalHelper::getSymbolScope(Symbol *sym)
{
    auto it = symbol_scopes.find(sym);

    if ( it == std::end(symbol_scopes) )
	return nullptr;

    return it->second;
}

void GlobalHelper::setSymbolScope(Symbol *sym, Scope *scope)
{
    symbol_scopes[sym] = scope;
}

int GlobalHelper::getDefinitionTime(Symbol *sym)
{
    auto it = definition_time.find(sym);

    if ( it == std::end(definition_time) )
	throw 1;

    return it->second;
}
