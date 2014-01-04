#include "referencetype.hpp"

ReferenceType::ReferenceType(Type *referred_type) : referred_type(referred_type)
{
    
}

Type* ReferenceType::getReferredType()
{
    return referred_type;
}
