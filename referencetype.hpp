#ifndef _REFERENCETYPE_HPP_
#define _REFERENCETYPE_HPP_

#include "type.hpp"

class ReferenceType : public Type
{
public:

    ReferenceType(const Type *type);

    std::string getName() const override;
    size_t sizeOf() const override;
    TypeKind getTypeKind() const override;

    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *type) const override;

    bool isReference() const override;
    bool isConst() const override;

    FunctionSymbol* getConversionTo(const Type *type) const override;

    const Type* getUnqualifiedType() const override;

    const Type *type;
};

#endif
