#ifndef _REFERENCETYPE_HPP_
#define _REFERENCETYPE_HPP_

#include "type.hpp"
#include "globalconfig.hpp"

class ReferenceType : public Type 
{
public:

    ReferenceType(Type *referred_type);

    Type* getReferredType() const;

    virtual string getName() const;
    virtual int getSize() const;

    virtual bool isReference() const;
    
private:
    
    Type *referred_type;
    
};

#endif
