#include "pointertype.hpp"
#include "builtins.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "typefactory.hpp"

PointerType::PointerType(const Type *type) : type(type)
{

}

std::string PointerType::getName() const { return type -> getName() + "~ptr"; }
size_t PointerType::sizeOf() const { return Comp::config().int_size; }

TypeKind PointerType::getTypeKind() const { return TypeKind::POINTER; }

bool PointerType::isConvertableTo(const Type *t) const
{
    return (this == t -> removeRef()) || (t == TypeFactory::getPointer(BuiltIns::void_type.get()));
}

boost::optional<int> PointerType::rankOfConversion(const Type *t) const
{
    if ( !isConvertableTo(t) )
        return boost::none;

    return (this == t ? 0 : 1);
}

bool PointerType::isReference() const { return false; } 

const FunctionSymbol* PointerType::getConversionTo(const Type *) const { return nullptr; }
const Type* PointerType::pointedType() const { return type; }
