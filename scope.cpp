#include "scope.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"
#include "variablenode.hpp"
#include "typefactory.hpp"

Scope::~Scope() 
{

}

bool Scope::isUnsafeBlock() const
{
	return false;
}

void Scope::define(Symbol *sym)
{
	accept(sym -> getScopeVisitor());
}

const Type* Scope::resolveType(std::string name)
{
	auto scope = this;

	Symbol *sym = nullptr;

	while ( true )
	{
		while ( scope != nullptr && scope -> resolve(name) == sym )
			scope = scope -> getEnclosingScope();

		if ( scope == nullptr ) 
			return nullptr;

		sym = scope -> resolve(name);

		if ( dynamic_cast<Type*>(sym) != nullptr )
			return dynamic_cast<Type*>(sym);
	}
}

const Type* Scope::fromTypeInfo(TypeInfo type_info)
{
	return fromTypeInfo(type_info, getTemplateInfo());
}

const Type* Scope::fromTypeInfo(TypeInfo type_info, const TemplateInfo& template_info)
{
	auto type_name = type_info.type_name;

	if ( template_info.sym && template_info.sym -> isIn(type_name) )
		type_name = boost::get<std::string>(*template_info.getReplacement(type_name));

	const Type *type = resolveType(type_name);

	if ( type_info.pointer_depth > 0 && !isUnsafeBlock() )
		throw SemanticError("Using pointer type in safe block " + getScopeName() + ".");
	
	if ( type == nullptr )
		throw SemanticError(type_name + " is not a type");

	if ( dynamic_cast<const TemplateStructSymbol*>(type) )
	{
		auto tmpl = dynamic_cast<const TemplateStructSymbol*>(type);
		
		if ( type_info.template_params.size() != tmpl -> template_symbols.size() )
			throw SemanticError("Wrong number of template parameters");

		auto getTemplateParam = [] ( TemplateParamInfo tp_info )
		{
			if ( tp_info.which() == 1 )
				return TemplateParam(boost::get<TypeInfo>(tp_info));
			else
				return TemplateParam(*boost::get<ExprNode*>(tp_info) -> getCompileTimeValue());
		};

		std::vector<TemplateParam> tmpl_params(type_info.template_params.size());
		std::transform(std::begin(type_info.template_params), std::end(type_info.template_params), std::begin(tmpl_params), getTemplateParam);

		auto sym = tmpl -> getSpec(tmpl_params, this);	
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
