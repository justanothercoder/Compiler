#ifndef _CONSTTYPE_HPP_
#define _CONSTTYPE_HPP_

#include "type.hpp"

class ConstType : public Type
{
public:
	
	ConstType(const Type *type);
    
	std::string getName() const override;
    size_t getSize() const override;
    
    TypeKind getTypeKind() const override;

	bool isConvertableTo(const Type *t) const override;
	boost::optional<int> rankOfConversion(const Type *t) const override;

	bool isReference() const override;
	bool isConst() const override;

	FunctionSymbol* getConversionTo(const Type *t) const override;

private:

	const Type *type;
};

#endif
