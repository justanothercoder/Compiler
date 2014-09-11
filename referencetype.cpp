#include "referencetype.hpp"

#include "globalconfig.hpp"

ReferenceType::ReferenceType(const Type *type) : type(type)
{

}

std::string ReferenceType::getName() const 
{
//	return type -> getName() + "&";
	return type -> getName() + "~ref";
}

size_t ReferenceType::getSize() const 
{
	return GlobalConfig::int_size;
}

TypeKind ReferenceType::getTypeKind() const
{
	return TypeKind::REFERENCE;
}

bool ReferenceType::isConvertableTo(const Type *t) const 
{
	return (this == t) || type -> isConvertableTo(t);
}

boost::optional<int> ReferenceType::rankOfConversion(const Type *t) const 
{
	if ( !type -> isConvertableTo(t) )
		return boost::none;

	return 1 + *type -> rankOfConversion(t);
}

bool ReferenceType::isReference() const 
{
	return true;
}

bool ReferenceType::isConst() const 
{
	return type -> isConst();
}
	
FunctionSymbol* ReferenceType::getConversionTo(const Type *) const
{
	return nullptr;
}
	
const Type* ReferenceType::getUnqualifiedType() const 
{
	return type -> getUnqualifiedType();
}
	
const Symbol* ReferenceType::getSymbol() const
{
	return type -> getSymbol();
}
