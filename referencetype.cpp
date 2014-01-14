#include "referencetype.hpp"

ReferenceType::ReferenceType(Type *referred_type) : referred_type(referred_type)
{
    
}

Type* ReferenceType::getReferredType() const
{
    return referred_type;
}

string ReferenceType::getName() const
{
    return referred_type->getName() + "~ref";
}

int ReferenceType::getSize() const
{
    return sizeof(int*);
}
