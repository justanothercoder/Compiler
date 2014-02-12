#include "typehelper.hpp"
#include "classvariablesymbol.hpp"

map<Type*, ReferenceType*> TypeHelper::references = map<Type*, ReferenceType*>();

ReferenceType* TypeHelper::getReferenceType(Type *target)
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
	if ( lhs == rhs )
		return true;

	if ( lhs->isReference() )
		lhs = static_cast<ReferenceType*>(lhs)->getReferredType();

	return existsConversion(lhs, rhs);
}

bool TypeHelper::existsConversion(Type *lhs, Type *rhs)
{
	if ( lhs == rhs )
		return true;

	if ( dynamic_cast<StructSymbol*>(rhs) )
		return getConversion(lhs, rhs) == nullptr ? lhs == rhs : true;

	return lhs == rhs;
}

FunctionSymbol* TypeHelper::getConversion(Type *lhs, Type *rhs)
{
	if ( rhs->getTypeKind() != TypeKind::STRUCT )
		return nullptr;

	StructSymbol *struc = static_cast<StructSymbol*>(rhs);

	string lhs_name = lhs->getName();    
	string rhs_name = rhs->getName();

	VariableSymbol *_conversion = dynamic_cast<VariableSymbol*>(struc->resolve(rhs_name));
	OverloadedFunctionSymbol *conversion = dynamic_cast<OverloadedFunctionSymbol*>(_conversion->getType());

	auto overloads = FunctionHelper::getBestOverload(conversion->getTypeInfo().overloads, {getReferenceType(rhs), lhs});

	return overloads.empty() ? nullptr : conversion->getTypeInfo().symbols[*std::begin(overloads)];
}

Type* TypeHelper::fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateStructSymbol *template_sym, vector<ExprNode*> expr)
{    
	auto type_name = type_info.getTypeName();

	if ( template_sym && template_sym->isIn(type_name) )
		type_name = static_cast<ClassVariableSymbol*>(static_cast<ReferenceType*>(template_sym->getReplacement(type_name, expr)->getType())->getReferredType())->sym->getName();

	Symbol *sym = scope->resolve(type_info.getTypeName());

	if ( sym == nullptr )
		throw SemanticError("No such symbol " + type_info.getTypeName());

	Type *type = dynamic_cast<Type*>(sym);

	if ( type == nullptr )
		throw SemanticError(type_info.getTypeName() + " is not a type");

	if ( type_info.getTemplateParams().size() > 0 )
	{
		auto tmpl = dynamic_cast<TemplateStructSymbol*>(type);
		auto sym = tmpl->getSpec(type_info.getTemplateParams());	
		type = dynamic_cast<Type*>(sym);
	}

	if ( type_info.getIsRef() )
		type = TypeHelper::getReferenceType(type);

	return type;
}
