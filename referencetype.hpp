#ifndef _REFERENCETYPE_HPP_
#define _REFERENCETYPE_HPP_

#include "type.hpp"

class ReferenceType : public Type
{
public:

    ReferenceType(const Type* type);

    std::string typeName() const override;
    size_t sizeOf() const override;

    bool isConvertableTo(const Type* type) const override;
    boost::optional<int> rankOfConversion(const Type* type) const override;

    bool isReference() const override;

    const FunctionalSymbol* getConversionTo(const Type* type) const override;
    const Type* removeRef() const override;

private:
    const Type* type;
};

#endif
