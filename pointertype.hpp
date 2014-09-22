#ifndef _POINTERTYPE_HPP_
#define _POINTERTYPE_HPP_

#include "type.hpp"

class PointerType : public Type
{
public:

	PointerType(const Type *type);
	
	void accept(TypeVisitor *visitor) const override;

	std::string getName() const override;
	size_t getSize() const override;
	TypeKind getTypeKind() const override;
	
	bool isConvertableTo(const Type *type) const override;
	boost::optional<int> rankOfConversion(const Type *t) const override;

	bool isReference() const override;
	bool isConst() const override;

	FunctionSymbol* getConversionTo(const Type *t) const override;

	const Symbol* getSymbol() const override;

	const Type* getPointedType() const;

private:

	const Type *type;
};

#endif
