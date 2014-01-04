#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, bool is_operator) : name(name), function_type_info(function_type_info), enclosing_scope(enclosing_scope), is_operator(is_operator)
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
	    members[sym->getName()] = new OverloadedFunctionSymbol(sym->getName(), OverloadedFunctionTypeInfo({ }));

	OverloadedFunctionSymbol* ov_func = dynamic_cast<OverloadedFunctionSymbol*>(members[sym->getName()]);
	
	if ( ov_func == nullptr )
	    throw SemanticError(sym->getName() + " is not a function");
	
	OverloadedFunctionTypeInfo type_info = ov_func->getTypeInfo();
	
	FunctionTypeInfo func_type_info = static_cast<FunctionSymbol*>(sym)->getTypeInfo();
	type_info.overloads.insert(func_type_info);
	type_info.symbols[func_type_info] = static_cast<FunctionSymbol*>(sym);
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	members[sym->getName()] = sym;
	addresses[static_cast<VariableSymbol*>(sym)] = (scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize());
    }
    else
	members[sym->getName()] = sym;
}

void FunctionSymbol::setTypeInfo(FunctionTypeInfo function_type_info)
{
    this->function_type_info = function_type_info;
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

    for ( int i = 0; i < function_type_info.getNumberOfParams(); ++i )
	res += "_" + function_type_info.getParamType(i)->getName();

    return res;
}

int FunctionSymbol::getAddress(VariableSymbol *sym)
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

Scope* FunctionSymbol::getScope()
{
    return symbol_scope;
}

void FunctionSymbol::setScope(Scope *scope)
{
    symbol_scope = scope;
}

string FunctionSymbol::getName()
{
    return name;
}

int FunctionSymbol::getSize()
{
    return sizeof(int*);
}

FunctionTypeInfo FunctionSymbol::getTypeInfo()
{
    return function_type_info;
}
