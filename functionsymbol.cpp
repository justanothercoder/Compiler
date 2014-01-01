#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name, FunctionType *function_type, Scope *enclosing_scope) : TypedSymbol(name), function_type(function_type), enclosing_scope(enclosing_scope)
{
    
}

Scope* FunctionSymbol::getEnclosingScope()
{
    return enclosing_scope;
}

Symbol* FunctionSymbol::resolve(string name)
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

void FunctionSymbol::define(Symbol *sym)
{
    if ( members.find(name) == std::end(members) )
	members[sym->getName()] = sym;
}

Type* FunctionSymbol::getType()
{
    return function_type;
}

void FunctionSymbol::setType(FunctionType *function_type)
{
    this->function_type = function_type;
}

Type* FunctionSymbol::getTypeHint(ExprNode *expr)
{
    return type_hints[expr];
}

void FunctionSymbol::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}

string FunctionSymbol::getTypedName()
{
    string res = name;

    for ( int i = 0; i < function_type->getNumberOfParams(); ++i )
	res += "_" + function_type->getParamType(i)->getName();

    return res;
}

int FunctionSymbol::getAddress(VariableSymbol *sym)
{
    
}
