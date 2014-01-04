#ifndef _REFERENCETYPE_HPP_
#define _REFERENCETYPE_HPP_

#include "type.hpp"

class ReferenceType : public Type 
{
public:

    ReferenceType(Type *referred_type);

    Type* getReferredType();
    
private:
    
    Type *referred_type;
    
};

#endif
