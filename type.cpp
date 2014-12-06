#include "type.hpp"

Type::~Type()
{

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
