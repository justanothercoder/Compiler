#include "type.hpp"

Type::~Type()
{

}

const Type* Type::removeRef() const
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
