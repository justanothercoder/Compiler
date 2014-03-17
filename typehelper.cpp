#include "typehelper.hpp"
#include "classvariablesymbol.hpp"

map<Type*, ReferenceType*> TypeHelper::references = map<Type*, ReferenceType*>();

ReferenceType* TypeHelper::addReference(Type *target)
{
	if ( target->isReference() )
		return static_cast<ReferenceType*>(target);

	auto it = references.find(target);

	if ( it == std::end(references) )
		references[target] = new ReferenceType(target);

	return references[target];
}

bool TypeHelper::isConvertable(Type *lhs, Type *rhs)
{
	auto _lhs = TypeHelper::removeReference(lhs);
	auto _rhs = TypeHelper::removeReference(rhs);

	if ( rhs->isReference() )
		return _lhs == _rhs;
	else
		return (_lhs == _rhs) ? true : (TypeHelper::getConversion(lhs, rhs) != nullptr);
/*
	if ( lhs == rhs )
		return true;

	return existsConversion(TypeHelper::removeReference(lhs), rhs);
*/
}

bool TypeHelper::existsConversion(Type *lhs, Type *rhs)
{
	if ( lhs == rhs )
		return true;

	if ( dynamic_cast<StructSymbol*>(rhs) )
		return getConversion(lhs, rhs) == nullptr ? lhs == rhs : true;

	return false;
}

FunctionSymbol* TypeHelper::getConversion(Type *lhs, Type *rhs)
{
	if ( rhs->getTypeKind() != TypeKind::STRUCT )
		return nullptr;

	auto struc = static_cast<StructSymbol*>(rhs);

	string lhs_name = lhs->getName();    
	string rhs_name = rhs->getName();

	OverloadedFunctionSymbol *cast_op = nullptr, *conversion = nullptr;

	try
	{
		cast_op = CallHelper::getOverloadedMethod("operator " + rhs_name, static_cast<StructSymbol*>(lhs));
	}
	catch ( ... )
	{

	}
	
	try 
	{
		conversion = CallHelper::getOverloadedMethod(rhs_name, struc);
	}
	catch ( ... )
	{

	}

	if ( cast_op == nullptr && conversion == nullptr )
		throw SemanticError("No conversion from '" + lhs_name + "' to '" + rhs_name + "'.");

	if ( cast_op != nullptr )
		return cast_op->getTypeInfo().symbols[FunctionTypeInfo(rhs, {addReference(lhs)})];

	return conversion->getTypeInfo().symbols[FunctionTypeInfo(addReference(rhs), {addReference(rhs), lhs})];
}

FunctionSymbol* TypeHelper::getCopyConstructor(Type *type)
{
	if ( type->getTypeKind() != TypeKind::STRUCT )
		return nullptr;

	StructSymbol *struc = static_cast<StructSymbol*>(type);

	auto type_name = struc->getName();

	auto constructor = CallHelper::getOverloadedMethod(type_name, struc);

	return FunctionHelper::getViableOverload(constructor, {addReference(type), addReference(type)});
}

Type* TypeHelper::fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateStructSymbol *template_sym, vector<ExprNode*> expr)
{    
	auto type_name = type_info.type_name;

	if ( template_sym && template_sym->isIn(type_name) )
		type_name = static_cast<ClassVariableSymbol*>(TypeHelper::removeReference(template_sym->getReplacement(type_name, expr)->getType()))->sym->getName();

//	Symbol *sym = scope->resolve(type_info.getTypeName());
//	auto type = TypeHelper::resolveType(type_info.type_name, scope);
	auto type = TypeHelper::resolveType(type_name, scope);

	if ( type == nullptr )
		throw SemanticError(type_info.type_name + " is not a type");

	if ( type_info.template_params.size() > 0 )
	{
		auto tmpl = dynamic_cast<TemplateStructSymbol*>(type);
		auto sym = tmpl->getSpec(type_info.template_params);	
		type = dynamic_cast<Type*>(sym);
	}

	if ( type_info.is_ref )
		type = TypeHelper::addReference(type);

	return type;
}
	
Type* TypeHelper::removeReference(Type *t)
{
	if ( t->isReference() )
		t = static_cast<ReferenceType*>(t)->getReferredType();
	return t;
}
	
Type* TypeHelper::resolveType(string name, Scope *sc)
{
	auto scope = sc;

	Symbol *_ = nullptr;

	while ( true )
	{
		while ( scope != nullptr && scope->resolve(name) == _ )
			scope = scope->getEnclosingScope();

		if ( scope == nullptr ) 
			return nullptr;

		_ = scope->resolve(name);

		if ( dynamic_cast<Type*>(_) != nullptr )
			return dynamic_cast<Type*>(_);		
	}
}
