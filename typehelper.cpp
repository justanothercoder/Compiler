#include "typehelper.hpp"

map<Type*, ReferenceType*> TypeHelper::references = map<Type*, ReferenceType*>();

ReferenceType* TypeHelper::getReferenceType(Type *target)
{
    auto it = references.find(target);

    if ( it == std::end(references) )
	references[target] = new ReferenceType(target);

    return references[target];
}

bool TypeHelper::isConvertable(Type *lhs, Type *rhs)
{
    if ( dynamic_cast<ReferenceType*>(lhs) )
	lhs = dynamic_cast<ReferenceType*>(lhs);

    return (lhs == rhs);
}
