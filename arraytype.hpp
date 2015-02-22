#ifndef _ARRAYTYPE_HPP_
#define _ARRAYTYPE_HPP_

#include "type.hpp"

class ArrayType : public Type
{
public:

    ArrayType(const Type* type, int size);

    std::string typeName() const override;
    size_t sizeOf() const override;

    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *t) const override;

    bool isArray() const override;
    bool isReference() const override;

    const FunctionalSymbol* getConversionTo(const Type *t) const override;

    const Type* pointedType() const;
    int sizeOfArray() const;

private:

    const Type* type;
    int size;
};

#endif
