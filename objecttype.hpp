#ifndef _OBJECTTYPE_HPP_
#define _OBJECTTYPE_HPP_

#include "functionaltype.hpp"

class VarSymbol;
class FunctionalSymbol;

class ObjectType : public Type
{
public:
    
    bool isObjectType() const override;

    virtual const Symbol*           resolveMember(const std::string& name) const = 0;
    virtual const FunctionalSymbol* resolveMethod(const std::string& name, const FunctionTypeInfo& info) const = 0;

    int offsetOf(const std::string& name) const;
    
    bool hasConversionConstructor(const ObjectType* st) const;
    bool hasConversionOperator(const ObjectType* st) const;
    
    const FunctionalSymbol* getConversionConstructor(const ObjectType *st) const;
    const FunctionalSymbol* getConversionOperator(const ObjectType *st) const;
    
    const FunctionalSymbol* getCopyConstructor() const;
    const FunctionalSymbol* getDefaultConstructor() const;

    const FunctionalSymbol* methodWith(const std::string& name, FunctionTypeInfo ft) const;
    const FunctionalSymbol* constructorWith(FunctionTypeInfo ft) const;

    virtual std::vector<VarSymbol*> members() const = 0;
    virtual std::vector<FunctionalSymbol*> methods() const = 0;
};

#endif
