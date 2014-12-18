#ifndef _FUNCTIONTYPE_HPP_
#define _FUNCTIONTYPE_HPP_

#include "type.hpp"
#include "functiontypeinfo.hpp"

class FunctionType
{
public:

    FunctionType(VariableType return_type, FunctionTypeInfo type_info);

    std::string getName() const;

    VariableType returnType() const;
    const FunctionTypeInfo& typeInfo() const;

//    bool isConvertableTo(const Type *type) const override;
//    boost::optional<int> rankOfConversion(const Type *type) const override;

//    FunctionSymbol* getConversionTo(const Type *type) const override;

//    TypeKind getTypeKind() const override;
//    size_t getSize() const override;

private:

    VariableType return_type;
    FunctionTypeInfo type_info;
};

#endif
