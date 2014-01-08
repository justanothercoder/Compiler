#include "overloadedfunctionsymbol.hpp"

OverloadedFunctionSymbol::OverloadedFunctionSymbol(string name, OverloadedFunctionTypeInfo type_info, bool is_method) : name(name), type_info(type_info), is_method(is_method)
{
    
}

string OverloadedFunctionSymbol::getName()
{
    return name;   
}

int OverloadedFunctionSymbol::getSize()
{
    return sizeof(int*);
}

Scope* OverloadedFunctionSymbol::getScope()
{
    return symbol_scope;
}

void OverloadedFunctionSymbol::setScope(Scope *scope)
{
    symbol_scope = scope;
}

OverloadedFunctionTypeInfo OverloadedFunctionSymbol::getTypeInfo()
{
    return type_info;
}

void OverloadedFunctionSymbol::addOverload(FunctionTypeInfo func_type_info, FunctionSymbol *sym)
{
    type_info.overloads.insert(func_type_info);
    type_info.symbols[func_type_info] = sym;
}

bool OverloadedFunctionSymbol::isMethod()
{
    return is_method;
}

Type* OverloadedFunctionSymbol::getBaseType()
{
    if ( !isMethod() )
	throw;    
    return type_info.overloads.begin()->getParamType(0);
}
