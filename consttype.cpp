#include "consttype.hpp"
#include "globalconfig.hpp"

ConstType::ConstType(const Type *type) : type(type)
{

}

std::string ConstType::getName() const
{
//	return "const " + type -> getName();
	return "const~" + type -> getName();
}

size_t ConstType::getSize() const
{
	return GlobalConfig::int_size;
}

TypeKind ConstType::getTypeKind() const
{
	return TypeKind::CONSTTYPE;
}

bool ConstType::isConvertableTo(const Type *t) const
{
	return type -> isConvertableTo(t) /*&& getConversionTo(t) -> is_const*/;
}

boost::optional<int> ConstType::rankOfConversion(const Type *t) const
{
	if ( !type -> isConvertableTo(t) )
		return boost::none;

	return 1 + *type -> rankOfConversion(t);
}

bool ConstType::isReference() const
{
	return type -> isReference();
}

bool ConstType::isConst() const
{
	return true;
}

FunctionSymbol* ConstType::getConversionTo(const Type *t) const
{
	return type -> getConversionTo(t);
}
	
const Type* ConstType::getUnqualifiedType() const
{
	return type -> getUnqualifiedType();
}
	
const Symbol* ConstType::getSymbol() const
{
	return type -> getSymbol();
}

void ConstType::accept(TypeVisitor *visitor) const
{
	visitor -> visit(this);
}
