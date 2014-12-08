#include "arraytype.hpp"
#include "builtins.hpp"
#include "globalconfig.hpp"
#include "typefactory.hpp"

ArrayType::ArrayType(const Type *type, int size) : type(type), size(size)
{

}

std::string ArrayType::getName() const
{
    return type -> getName() + "~arr" + std::to_string(size);
}

size_t ArrayType::sizeOf() const
{
    return type -> sizeOf() * size;
}

TypeKind ArrayType::getTypeKind() const
{
    return TypeKind::ARRAY;
}

bool ArrayType::isConvertableTo(const Type *t) const
{
    return (this == t -> getUnqualifiedType());
}

boost::optional<int> ArrayType::rankOfConversion(const Type *t) const
{
    if ( !isConvertableTo(t) )
        return boost::none;

    return (this == t ? 0 : 1);
}

bool ArrayType::isReference() const
{
    return false;
}

bool ArrayType::isConst() const
{
    return false;
}

FunctionSymbol* ArrayType::getConversionTo(const Type *) const
{
    return nullptr;
}

const Type* ArrayType::getPointedType() const
{
    return type;
}
