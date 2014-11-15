#ifndef _FUNCTIONTYPE_HPP_
#define _FUNCTIONTYPE_HPP_

#include "type.hpp"
#include "functiontypeinfo.hpp"

class FunctionType : public Type
{
public:

    FunctionType(const Type *return_type, FunctionTypeInfo type_info);

    std::string getName() const override;

    const Type* getReturnType() const;
    const FunctionTypeInfo& getTypeInfo() const;

    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *type) const override;

    FunctionSymbol* getConversionTo(const Type *type) const override;

    const Symbol* getSymbol() const override;

    TypeKind getTypeKind() const override;
    size_t getSize() const override;

private:

    const Type *return_type;
    FunctionTypeInfo type_info;
};

#endif
