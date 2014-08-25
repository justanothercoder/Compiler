#ifndef _POINTERTYPE_HPP_
#define _POINTERTYPE_HPP_

#include "type.hpp"

class PointerType : public Type
{
public:

	PointerType(Type *type);

	std::string getName() const override;
	size_t getSize() const override;
	TypeKind getTypeKind() const override;

private:

	Type *type;
};

#endif
