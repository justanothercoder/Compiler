#include "type.hpp"

Type::~Type() 
{

}

const Symbol* Type::getSymbol() const
{
	return nullptr;
}

const Type* Type::getUnqualifiedType() const
{
	return this;
}
	
bool Type::isReference() const
{
	return false;
}

bool Type::isConst() const
{
	return false;
}

