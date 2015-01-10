#ifndef _OBJECTTYPE_HPP_
#define _OBJECTTYPE_HPP_

#include "functionaltype.hpp"

class VariableSymbol;

class ObjectType : public Type
{
public:
    
    bool isObjectType() const override;

    virtual Symbol* resolveMember(std::string name) const = 0;
    virtual FunctionalType* resolveMethod(std::string name) const = 0;
    virtual int offsetOf(VariableSymbol* sym) const = 0;
};

#endif
