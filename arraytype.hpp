#ifndef _ARRAYTYPE_HPP_
#define _ARRAYTYPE_HPP_

#include "type.hpp"

class ArrayType : public Type
{
public:

    ArrayType(const Type *type, int size);

    std::string getName() const override;
    size_t getSize() const override;
    TypeKind getTypeKind() const override;

    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *t) const override;

    bool isReference() const override;
    bool isConst() const override;

    FunctionSymbol* getConversionTo(const Type *t) const override;

    const Type* getPointedType() const;

    const Type *type;
    int size;
};

#endif
