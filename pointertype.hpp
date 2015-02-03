#ifndef _POINTERTYPE_HPP_
#define _POINTERTYPE_HPP_

#include "type.hpp"

class PointerType : public Type
{
public:

    PointerType(const Type* type);

    std::string getName() const override;
    size_t sizeOf() const override;

    bool isConvertableTo(const Type* type) const override;
    boost::optional<int> rankOfConversion(const Type* t) const override;

    bool isPointer() const override;
    bool isReference() const override;

    const FunctionSymbol* getConversionTo(const Type* t) const override;
    const Type* pointedType() const;

private:

    const Type* type;
};

#endif
