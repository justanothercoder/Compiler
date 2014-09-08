#include "typehelper.hpp"
#include "classvariablesymbol.hpp"
#include "templatestructsymbol.hpp"
#include "variablenode.hpp"
#include "typefactory.hpp"

const Type* TypeHelper::fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateInfo& template_info)
{    
	auto type_name = type_info.type_name;

	if ( template_info.sym && template_info.sym -> isIn(type_name) )
		type_name = boost::get<std::string>(*template_info.getReplacement(type_name));

	const Type *type = TypeHelper::resolveType(type_name, scope);

	if ( type_info.pointer_depth > 0 && !scope -> isUnsafeBlock() )
		throw SemanticError("Using pointer type in safe block.");
	
	if ( type == nullptr )
		throw SemanticError(type_name + " is not a type");

	if ( dynamic_cast<const TemplateStructSymbol*>(type) )
	{
		auto tmpl = dynamic_cast<const TemplateStructSymbol*>(type);
		
		if ( type_info.template_params.size() != tmpl -> template_symbols.size() )
			throw SemanticError("Wrong number of template parameters");

		auto getTemplateParam = [] ( ExprNode *expr )
		{
			if ( dynamic_cast<VariableNode*>(expr) != nullptr 
			  && dynamic_cast<ClassVariableSymbol*>(static_cast<VariableNode*>(expr) -> variable) != nullptr )
				return TemplateParam(static_cast<ClassVariableSymbol*>(static_cast<VariableNode*>(expr) -> variable) -> getName());
			else
				return TemplateParam(*expr -> getCompileTimeValue());
		};

		std::vector<TemplateParam> tmpl_params(type_info.template_params.size());
		std::transform(std::begin(type_info.template_params), std::end(type_info.template_params), std::begin(tmpl_params), getTemplateParam);

		auto sym = tmpl -> getSpec(tmpl_params);	
		type = dynamic_cast<Type*>(sym);
	}

	if ( type_info.is_ref )
		type = TypeFactory::getReference(type);

	if ( type_info.is_const )
		type = TypeFactory::getConst(type);

	for ( int i = 0; i < type_info.pointer_depth; ++i )
		type = TypeFactory::getPointer(type);
	
	return type;
}
	
Type* TypeHelper::resolveType(std::string name, Scope *sc)
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
