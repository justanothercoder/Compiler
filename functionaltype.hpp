#ifndef _FUNCTIONALTYPE_HPP_
#define _FUNCTIONALTYPE_HPP_

#include "valueinfo.hpp"
#include "type.hpp"
#include "callinfo.hpp"

class FunctionalType : public Type
{
public:
    
    size_t sizeOf() const override;
    TypeKind getTypeKind() const override;
    
    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *type) const override;
    FunctionSymbol* getConversionTo(const Type *type) const override;

    virtual CallInfo resolveCall(std::vector<ValueInfo> arguments) const = 0;
};

#endif
