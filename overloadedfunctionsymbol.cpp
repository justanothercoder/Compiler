#include "overloadedfunctionsymbol.hpp"

OverloadedFunctionSymbol::OverloadedFunctionSymbol(string name, OverloadedFunctionTypeInfo type_info, FunctionTraits traits) : name(name), type_info(type_info), traits(traits) { }

string OverloadedFunctionSymbol::getName() const { return name; }
int OverloadedFunctionSymbol::getSize() const { return GlobalConfig::int_size; }

OverloadedFunctionTypeInfo OverloadedFunctionSymbol::getTypeInfo() const { return type_info; }

void OverloadedFunctionSymbol::addOverload(FunctionTypeInfo func_type_info, FunctionSymbol *sym)
{
    type_info.overloads.insert(func_type_info);
    type_info.symbols[func_type_info] = sym;
}

bool OverloadedFunctionSymbol::isMethod() const { return traits.is_method; }
bool OverloadedFunctionSymbol::isConstructor() const { return traits.is_constructor; }
bool OverloadedFunctionSymbol::isOperator() const { return traits.is_operator; }

VariableType OverloadedFunctionSymbol::getBaseType() const
{
    if ( !isMethod() )
		throw;    
    return std::begin(type_info.overloads)->params_types[0];
}

SymbolType OverloadedFunctionSymbol::getSymbolType() const { return SymbolType::OVERLOADED_FUNCTION; }
TypeKind OverloadedFunctionSymbol::getTypeKind() const { return TypeKind::OVERLOADEDFUNCTION; }
