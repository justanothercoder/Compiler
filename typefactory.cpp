#include "typefactory.hpp"

#include "pointertype.hpp"
#include "referencetype.hpp"
#include "consttype.hpp"

const Type* TypeFactory::getPointer(const Type *type)
{
	static std::map<const Type*, const Type*> pointers;

	if ( type -> getTypeKind() == TypeKind::REFERENCE )
		return nullptr;

	auto it = pointers.find(type);

	if ( it == std::end(pointers) )
		pointers[type] = new PointerType(type);

	return pointers[type];
}

const Type* TypeFactory::getReference(const Type *type)
{
	static std::map<const Type*, const Type*> references;
	
	if ( type -> getTypeKind() == TypeKind::REFERENCE )
		return nullptr;

	auto it = references.find(type);

	if ( it == std::end(references) )
		references[type] = new ReferenceType(type);

	return references[type];
}

const Type* TypeFactory::getConst(const Type *type)
{
	static std::map<const Type*, const Type*> consts;

	if ( type -> getTypeKind() == TypeKind::CONSTTYPE )
		return type;

	auto it = consts.find(type);

	if ( it == std::end(consts) )
		consts[type] = new ConstType(type);

	return consts[type];
}
