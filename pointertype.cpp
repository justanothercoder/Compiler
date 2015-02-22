#include "pointertype.hpp"
#include "builtins.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "typefactory.hpp"

PointerType::PointerType(const Type *type) : type(type) { }

std::string PointerType::typeName() const { return type -> typeName() + "~ptr"; }
size_t PointerType::sizeOf() const { return Comp::config().int_size; }

bool PointerType::isPointer() const { return true; }

bool PointerType::isConvertableTo(const Type *t) const
{
    return (this == t -> removeRef()) || (t == TypeFactory::getPointer(BuiltIns::void_type));
}

boost::optional<int> PointerType::rankOfConversion(const Type *t) const
{
    if ( !isConvertableTo(t) )
        return boost::none;

    return (this == t ? 0 : 1);
}

bool PointerType::isReference() const { return false; } 

const FunctionalSymbol* PointerType::getConversionTo(const Type *) const { return nullptr; }
const Type* PointerType::pointedType() const { return type; }
