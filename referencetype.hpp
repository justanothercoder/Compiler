#ifndef _REFERENCETYPE_HPP_
#define _REFERENCETYPE_HPP_

#include "type.hpp"

class ReferenceType : public Type
{
public:

	ReferenceType(Type *type);

	std::string getName() const override;
	size_t getSize() const override;
	TypeKind getTypeKind() const override;

private:

	Type *type;
};

#endif
