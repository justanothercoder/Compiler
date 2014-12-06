#include "overloadedfunctionsymbol.hpp"
#include "globalconfig.hpp"

OverloadedFunctionSymbol::OverloadedFunctionSymbol(std::string name
                                                 , OverloadedFunctionTypeInfo type_info
                                                 , FunctionTraits traits) : name     (name)
                                                                          , type_info(type_info)
                                                                          , traits   (traits)
{

}

std::string OverloadedFunctionSymbol::getName() const
{
    return name;
}

size_t OverloadedFunctionSymbol::getSize() const
{
    return GlobalConfig::int_size;
}

OverloadedFunctionTypeInfo OverloadedFunctionSymbol::getTypeInfo() const
{
    return type_info;
}

void OverloadedFunctionSymbol::addOverload(FunctionTypeInfo func_type_info, FunctionSymbol *sym) const
{
    type_info.overloads.insert(func_type_info);
    type_info.symbols[func_type_info] = sym;
}

bool OverloadedFunctionSymbol::isMethod() const
{
    return traits.is_method;
}
bool OverloadedFunctionSymbol::isConstructor() const
{
    return traits.is_constructor;
}
bool OverloadedFunctionSymbol::isOperator() const
{
    return traits.is_operator;
}

const Type* OverloadedFunctionSymbol::getBaseType() const
{
    if ( !isMethod() )
        throw;
    return std::begin(type_info.overloads) -> params_types[0];
}

SymbolType OverloadedFunctionSymbol::getSymbolType() const
{
    return SymbolType::OVERLOADED_FUNCTION;
}

TypeKind OverloadedFunctionSymbol::getTypeKind() const
{
    return TypeKind::OVERLOADEDFUNCTION;
}

FunctionSymbol* OverloadedFunctionSymbol::getViableOverload(FunctionTypeInfo params_type) const
{
    auto overloads = getTypeInfo().getBestOverload(params_type);
    return overloads.empty() ? nullptr : getTypeInfo().symbols.at(*std::begin(overloads));
}

bool OverloadedFunctionSymbol::isConvertableTo(const Type *) const
{
    return false;
}

boost::optional<int> OverloadedFunctionSymbol::rankOfConversion(const Type *) const
{
    return boost::none;
}

FunctionSymbol* OverloadedFunctionSymbol::getConversionTo(const Type *) const
{
    return nullptr;
}
