#include "referencetype.hpp"
#include "comp.hpp"
#include "globalconfig.hpp"

ReferenceType::ReferenceType(const Type *type) : type(type) { }

std::string ReferenceType::typeName() const { return type -> typeName() + "~ref"; } 
size_t ReferenceType::sizeOf() const { return Comp::config().int_size; } 

bool ReferenceType::isConvertableTo(const Type *t) const { return (this == t) || type -> isConvertableTo(t); }

boost::optional<int> ReferenceType::rankOfConversion(const Type *t) const
{
    if ( !type -> isConvertableTo(t) )
        return boost::none;

    return 1 + *type -> rankOfConversion(t);
}

bool ReferenceType::isReference() const { return true; }

const FunctionalSymbol* ReferenceType::getConversionTo(const Type *) const { return nullptr; }
const Type* ReferenceType::removeRef() const { return type; }
