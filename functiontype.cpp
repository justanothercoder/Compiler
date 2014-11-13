#include "functiontype.hpp"
#include "globalconfig.hpp"

FunctionType::FunctionType(const Type *return_type, FunctionTypeInfo type_info) : return_type(return_type), type_info(type_info)
{

}

std::string FunctionType::getName() const
{
    std::string res = "";

    res += return_type -> getName();
    res += "(";
    
    auto it = std::begin(type_info.params_types);
    res += (*it) -> getName();
    for ( ++it; it != std::end(type_info.params_types); ++it ) 
        res += ", " + (*it) -> getName();

    res += ")";
    return res;
}
    
const Type* FunctionType::getReturnType() const
{
    return return_type;
}

const FunctionTypeInfo& FunctionType::getTypeInfo() const
{
    return type_info;
}

size_t FunctionType::getSize() const
{
    return GlobalConfig::int_size;
}

TypeKind FunctionType::getTypeKind() const
{
    return TypeKind::FUNCTION;
}

bool FunctionType::isConvertableTo(const Type *) const
{
    return false;
}

boost::optional<int> FunctionType::rankOfConversion(const Type *) const
{
    return boost::none;
}

FunctionSymbol* FunctionType::getConversionTo(const Type *) const
{
    return nullptr;
}

const Symbol* FunctionType::getSymbol() const
{
    return nullptr;
}
