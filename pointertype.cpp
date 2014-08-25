#include "pointertype.hpp"

#include "globalconfig.hpp"

PointerType::PointerType(Type *type) : type(type)
{

}

std::string PointerType::getName() const
{
	return type -> getName() + "*";
}

size_t PointerType::getSize() const
{
	return GlobalConfig::int_size;
}

TypeKind PointerType::getTypeKind() const 
{
	return TypeKind::POINTER;
}
