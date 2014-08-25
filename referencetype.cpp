#include "referencetype.hpp"

#include "globalconfig.hpp"

ReferenceType::ReferenceType(Type *type) : type(type)
{

}

std::string ReferenceType::getName() const 
{
	return type -> getName() + "&";
}

size_t ReferenceType::getSize() const 
{
	return GlobalConfig::int_size;
}

TypeKind ReferenceType::getTypeKind() const
{
	return TypeKind::REFERENCE;
}

