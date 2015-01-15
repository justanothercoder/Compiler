#ifndef _OBJECTTYPE_HPP_
#define _OBJECTTYPE_HPP_

#include "functionaltype.hpp"

class VariableSymbol;

class ObjectType : public Type
{
public:
    
    bool isObjectType() const override;

    virtual const Symbol* resolveMember(const std::string& name) const = 0;
    virtual const FunctionalType* resolveMethod(const std::string& name) const = 0;
    virtual int offsetOf(const VariableSymbol* sym) const = 0;
};

#endif
