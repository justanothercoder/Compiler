#include "pointertype.hpp"
#include "builtins.hpp"
#include "globalconfig.hpp"
#include "typefactory.hpp"

PointerType::PointerType(const Type *type) : type(type)
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
	
bool PointerType::isConvertableTo(const Type *t) const 
{
	return (t == TypeFactory::getPointer(BuiltIns::void_type));
}

boost::optional<int> PointerType::rankOfConversion(const Type *t) const 
{
	if ( !isConvertableTo(t) )
		return boost::none;

	return 1;
}

bool PointerType::isReference() const
{
	return false;
}

bool PointerType::isConst() const
{
	return false;
}

FunctionSymbol* PointerType::getConversionTo(const Type *) const 
{
	return nullptr;
}
