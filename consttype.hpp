#ifndef _CONSTTYPE_HPP_
#define _CONSTTYPE_HPP_

#include "type.hpp"

class ConstType : public Type
{
	friend CopyTypeVisitor;

public:
	
	ConstType(const Type *type);

	void accept(TypeVisitor *visitor) const override;
    
	std::string getName() const override;
    size_t getSize() const override;
    
    TypeKind getTypeKind() const override;

	bool isConvertableTo(const Type *t) const override;
	boost::optional<int> rankOfConversion(const Type *t) const override;

	bool isReference() const override;
	bool isConst() const override;

	FunctionSymbol* getConversionTo(const Type *t) const override;

	const Type* getUnqualifiedType() const override;
	
	const Symbol* getSymbol() const;

private:

	const Type *type;
};

#endif
