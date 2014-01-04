#include "typehelper.hpp"

map<Type*, ReferenceType*> TypeHelper::references = map<Type*, ReferenceType*>();

ReferenceType* TypeHelper::getReferenceType(Type *target)
{
    if ( dynamic_cast<ReferenceType*>(target) )
	return dynamic_cast<ReferenceType*>(target);
    
    auto it = references.find(target);

    if ( it == std::end(references) )
	references[target] = new ReferenceType(target);

    return references[target];
}

bool TypeHelper::isConvertable(Type *lhs, Type *rhs)
{
    if ( lhs == rhs )
	return true;
    
    if ( dynamic_cast<ReferenceType*>(lhs) )
	lhs = dynamic_cast<ReferenceType*>(lhs)->getReferredType();

    return (lhs == rhs);
}

Type* TypeHelper::fromTypeInfo(TypeInfo type_info, Scope *scope)
{
    Symbol *sym = scope->resolve(type_info.type_name);

    if ( sym == nullptr )
	throw SemanticError("No such symbol " + type_info.type_name);

    Type *type = dynamic_cast<Type*>(sym);

    if ( type == nullptr )
	throw SemanticError(type_info.type_name + " is not a type");

    if ( type_info.is_ref )
	type = TypeHelper::getReferenceType(type);
    
    return type;
}
