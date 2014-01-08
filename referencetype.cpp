#include "referencetype.hpp"

ReferenceType::ReferenceType(Type *referred_type) : referred_type(referred_type)
{
    
}

Type* ReferenceType::getReferredType()
{
    return referred_type;
}

string ReferenceType::getName()
{
    return referred_type->getName() + "~ref";
}

int ReferenceType::getSize()
{
    return sizeof(int*);
}
