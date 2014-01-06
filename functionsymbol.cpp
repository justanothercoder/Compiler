#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(string name, FunctionTypeInfo function_type_info, Scope *enclosing_scope, bool is_operator) : name(name), function_type_info(function_type_info), enclosing_scope(enclosing_scope), is_operator(is_operator)
{
    scope_size = 0;
    params_size = sizeof(int*);
    
    scope_name = enclosing_scope->getScopeName() + "_" + name;
}

Scope* FunctionSymbol::getEnclosingScope()
{
    return enclosing_scope;
}

void FunctionSymbol::define(Symbol *sym)
{
    if ( dynamic_cast<FunctionSymbol*>(sym) != nullptr )
    {
	if ( table[sym->getName()] == nullptr )
	    table[sym->getName()] = new OverloadedFunctionSymbol(sym->getName(), OverloadedFunctionTypeInfo({ }));

	OverloadedFunctionSymbol* ov_func = dynamic_cast<OverloadedFunctionSymbol*>(table[sym->getName()]);
	
	if ( ov_func == nullptr )
	    throw SemanticError(sym->getName() + " is not a function");
	
	OverloadedFunctionTypeInfo type_info = ov_func->getTypeInfo();
	
	FunctionTypeInfo func_type_info = static_cast<FunctionSymbol*>(sym)->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(table[sym->getName()])->getType());
	ofs->addOverload(func_type_info, static_cast<FunctionSymbol*>(sym));
    }
    else if ( dynamic_cast<VariableSymbol*>(sym) != nullptr )
    {
	table[sym->getName()] = sym;
	if ( static_cast<VariableSymbol*>(sym)->isParam() )
	    addresses[static_cast<VariableSymbol*>(sym)] = -(params_size += static_cast<VariableSymbol*>(sym)->getType()->getSize());
	else
	    addresses[static_cast<VariableSymbol*>(sym)] = (scope_size += static_cast<VariableSymbol*>(sym)->getType()->getSize());
    }
    else
	table[sym->getName()] = sym;
}

void FunctionSymbol::setTypeInfo(FunctionTypeInfo function_type_info)
{
    this->function_type_info = function_type_info;
}

string FunctionSymbol::getTypedName()
{
    string res = name;

    for ( int i = 0; i < function_type_info.getNumberOfParams(); ++i )
	res += "_" + function_type_info.getParamType(i)->getName();

    return res;
}

string FunctionSymbol::getScopedTypedName()
{
    string res = scope_name;

    for ( int i = 0; i < function_type_info.getNumberOfParams(); ++i )
	res += "_" + function_type_info.getParamType(i)->getName();

    return res;
}

int FunctionSymbol::getScopeSize()
{
    return scope_size + params_size;
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
