#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name, FunctionType *function_type, Scope *enclosing_scope, bool is_operator) : TypedSymbol(name), function_type(function_type), enclosing_scope(enclosing_scope), is_operator(is_operator)
{
    scope_size = 0;
    
    scope_name = enclosing_scope->getScopeName() + "_" + name;
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
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	if ( members[sym->getName()] == nullptr )
	    members[sym->getName()] = new VariableSymbol(sym->getName(), new OverloadedFunctionType({ }));

	if ( dynamic_cast<OverloadedFunctionType*>(static_cast<VariableSymbol*>(members[sym->getName()])->getType()) == nullptr )
	    throw;
	
	OverloadedFunctionType *ot = static_cast<OverloadedFunctionType*>(static_cast<VariableSymbol*>(members[sym->getName()])->getType());
	
	FunctionType *ft = static_cast<FunctionType*>(static_cast<FunctionSymbol*>(sym)->getType());
	ot->overloads.insert(ft);
	ot->symbols[ft] = static_cast<FunctionSymbol*>(sym);
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	members[sym->getName()] = sym;
	addresses[static_cast<VariableSymbol*>(sym)] = (scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize());
    }
    else
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
    auto it = addresses.find(sym);

    if ( it == std::end(addresses) )
    {
	if ( getEnclosingScope() == nullptr )
	    throw;

	return getEnclosingScope()->getAddress(sym) - (scope_address - getEnclosingScope()->getScopeAddress());
    }

    return it->second;
}

int FunctionSymbol::getScopeAddress()
{
    return scope_address;
}

int FunctionSymbol::getScopeSize()
{
    return scope_size;
}

string FunctionSymbol::getScopeName()
{
    return scope_name;
}

void FunctionSymbol::recalc_scope_address()
{
    scope_address = enclosing_scope->getScopeAddress() + enclosing_scope->getScopeSize() + sizeof(int*);    
}

bool FunctionSymbol::isOperator()
{
    return is_operator;
}
