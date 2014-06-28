#include "typehelper.hpp"
#include "classvariablesymbol.hpp"

FunctionSymbol* TypeHelper::getConversion(VariableType lhs, VariableType rhs)
{
	if ( rhs.type -> getTypeKind() != TypeKind::STRUCT )
		return nullptr;

	auto struc = static_cast<StructSymbol*>(rhs.type);

	string lhs_name = lhs.getTypeName();    
	string rhs_name = rhs.getTypeName();

	OverloadedFunctionSymbol *cast_op = nullptr, *conversion = nullptr;

	try { cast_op = CallHelper::getOverloadedMethod("operator " + rhs_name, static_cast<StructSymbol*>(lhs.type)); }
	catch ( ... ) { }
	
	try { conversion = CallHelper::getOverloadedMethod(rhs_name, struc); }
	catch ( ... ) { }

	if ( cast_op == nullptr && conversion == nullptr )
		throw SemanticError("No conversion from '" + lhs_name + "' to '" + rhs_name + "'.");

	if ( cast_op != nullptr )
	{
		auto ref_lhs = lhs;
		ref_lhs.is_ref = true;

		return cast_op -> getTypeInfo().symbols.at({ref_lhs});
	}
	else
	{
		auto ref_rhs = rhs;
		ref_rhs.is_ref = true;

		return conversion -> getTypeInfo().symbols.at({ref_rhs, lhs});
	}
}

FunctionSymbol* TypeHelper::getCopyConstructor(VariableType type)
{
	if ( type.type -> getTypeKind() != TypeKind::STRUCT )
		return nullptr;

	type.is_ref = false;
	type.is_const = false;

	StructSymbol *struc = static_cast<StructSymbol*>(type.type);

	auto type_name = struc -> getName();

	auto constructor = CallHelper::getOverloadedMethod(type_name, struc);

	auto vt = type;
	vt.is_ref = true;

	auto cvt = vt;
	cvt.is_const = true;

	return FunctionHelper::getViableOverload(constructor, {vt, cvt});
}

FunctionSymbol* TypeHelper::getDefaultConstructor(VariableType type)
{
	if ( type.type -> getTypeKind() != TypeKind::STRUCT )
		return nullptr;

	type.is_ref = false;
	type.is_const = false;

	StructSymbol *struc = static_cast<StructSymbol*>(type.type);

	auto type_name = struc -> getName();

	auto constructor = CallHelper::getOverloadedMethod(type_name, struc);

	auto rt = type;
	rt.is_ref = true;

	return FunctionHelper::getViableOverload(constructor, {rt});
}

VariableType TypeHelper::fromTypeInfo(TypeInfo type_info, Scope *scope, TemplateInfo *template_info)
{    
	auto type_name = type_info.type_name;

	if ( template_info -> sym && template_info -> sym -> isIn(type_name) )
		type_name = template_info -> getReplacement(type_name) -> getType().type -> getName();

	auto type = VariableType(TypeHelper::resolveType(type_name, scope));
	
	if ( type.type == nullptr )
		throw SemanticError(type_name + " is not a type");

	if ( type_info.template_params.size() > 0 )
	{
		auto tmpl = dynamic_cast<TemplateStructSymbol*>(type.type);
		auto sym = tmpl -> getSpec(type_info.template_params);	
		type.type = dynamic_cast<Type*>(sym);
	}

	type.is_ref = type_info.is_ref; 
	type.is_const = type_info.is_const;

	return type;
}
	
Type* TypeHelper::resolveType(string name, Scope *sc)
{
	auto scope = sc;

	Symbol *_ = nullptr;

	while ( true )
	{
		while ( scope != nullptr && scope -> resolve(name) == _ )
			scope = scope -> getEnclosingScope();

		if ( scope == nullptr ) 
			return nullptr;

		_ = scope -> resolve(name);

		if ( dynamic_cast<Type*>(_) != nullptr )
			return dynamic_cast<Type*>(_);		
	}
}
