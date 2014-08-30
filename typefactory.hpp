#ifndef _TYPEFACTORY_HPP_
#define _TYPEFACTORY_HPP_

#include <map>

#include "referencetype.hpp"

class TypeFactory
{
public:

	static const Type* getPointer(const Type *type);
	static const Type* getReference(const Type *type);
	static const Type* getConst(const Type *type);

private:
	TypeFactory();
};

#endif
