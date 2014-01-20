#include "overloadedfunctionsymbol.hpp"

OverloadedFunctionSymbol::OverloadedFunctionSymbol(string name, OverloadedFunctionTypeInfo type_info, bool is_method) : name(name), type_info(type_info), is_method(is_method) { }

string OverloadedFunctionSymbol::getName() const { return name; }
int OverloadedFunctionSymbol::getSize() const { return GlobalConfig::int_size; }

OverloadedFunctionTypeInfo OverloadedFunctionSymbol::getTypeInfo() const { return type_info; }

void OverloadedFunctionSymbol::addOverload(FunctionTypeInfo func_type_info, FunctionSymbol *sym)
{
    type_info.overloads.insert(func_type_info);
    type_info.symbols[func_type_info] = sym;
}

bool OverloadedFunctionSymbol::isMethod() const { return is_method; }

Type* OverloadedFunctionSymbol::getBaseType() const
{
    if ( !isMethod() )
	throw;    
    return type_info.overloads.begin()->getParamType(0);
}

bool OverloadedFunctionSymbol::isReference() const { return false; }
SymbolType OverloadedFunctionSymbol::getSymbolType() const { return SymbolType::OVERLOADED_FUNCTION; }
