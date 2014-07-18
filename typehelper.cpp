#include "typehelper.hpp"
#include "classvariablesymbol.hpp"

VariableType TypeHelper::fromTypeInfo(TypeInfo type_info, Scope *scope, TemplateInfo *template_info)
{    
	auto type_name = type_info.type_name;

	if ( template_info -> sym && template_info -> sym -> isIn(type_name) )
		type_name = template_info -> getReplacement(type_name) -> getType().type -> getName();

	auto type = VariableType(TypeHelper::resolveType(type_name, scope));
	
	if ( type.type == nullptr )
		throw SemanticError(type_name + " is not a type");

//	if ( type_info.template_params.size() > 0 )
	if ( dynamic_cast<TemplateStructSymbol*>(type.type) )
	{
		auto tmpl = dynamic_cast<TemplateStructSymbol*>(type.type);
		
		if ( type_info.template_params.size() != tmpl -> template_symbols.size() )
			throw SemanticError("Wrong number of template parameters");

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
