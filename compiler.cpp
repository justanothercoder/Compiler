#include "compiler.hpp"
#include "scope.hpp"
#include "structdeclarationnode.hpp"
#include "typefactory.hpp"
#include "exprnode.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"


const Type* Compiler::fromTypeInfo(const TypeInfo& type_info, Scope *scope)
{
    return fromTypeInfo(type_info, scope -> getTemplateInfo(), scope);
}

const Type* Compiler::fromTypeInfo(const TypeInfo& type_info, const TemplateInfo& template_info, Scope *scope)
{
    auto type_name = type_info.type_name;

    if ( template_info.sym && template_info.sym -> isIn(type_name) )
        type_name = boost::get<std::string>(*template_info.getReplacement(type_name));

    const Type *type = scope -> resolveType(type_name);

    if ( type_info.pointer_depth > 0 && !scope -> isUnsafeBlock() )
        throw SemanticError("Using pointer type in safe block " + scope -> getScopeName() + ".");

    if ( type == nullptr )
        throw SemanticError(type_name + " is not a type");
    /*
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

    		auto sym = getSpec(tmpl, tmpl_params, scope);
    		type = dynamic_cast<const Type*>(sym);
    	}
    */
    if ( type_info.is_ref )
        type = TypeFactory::getReference(type);

    if ( type_info.is_const )
        type = TypeFactory::getConst(type);

    for ( int i = 0; i < type_info.pointer_depth; ++i )
        type = TypeFactory::getPointer(type);

    return type;

}
/*
DeclarationNode* Compiler::getSpecDecl(const TemplateStructSymbol *sym, std::vector<TemplateParam> template_params, Scope *scope)
{
	auto hash_func = [](std::vector<TemplateParam> vec)
	{
		unsigned long long P = 31, pow = 1, ans = 0;

		for ( size_t i = 0; i < vec.size(); ++i )
		{
			if ( vec[i].which() == 0 )
				ans += static_cast<int>(std::hash<std::string>()(boost::get<TypeInfo>(vec[i]).type_name) * pow);
			else
				ans += static_cast<int>(boost::get<int>(vec[i])) * pow;

			pow *= P;
		}

		return ans;
	};

	auto hash_ = hash_func(template_params);

//	auto it = sym -> specs.find(hash_);
//	if ( it != std::end(sym -> specs) )
//		return it -> second;

	auto children = sym -> holder -> getChildren();

	std::vector<AST*> vec;

	for ( auto t : children )
		vec.push_back(t -> copyTree());

	StructDeclarationNode *decl = new StructDeclarationNode(sym -> getName() + "~hash" + std::to_string(hash_), vec, *(new TemplateInfo(const_cast<TemplateStructSymbol*>(sym), tmpl_params)));

//	decl -> scope = new StructSymbol(getName(), holder -> scope, *(new TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols)));
	decl -> scope = scope;
//	decl -> definedSymbol = new StructSymbol(getName(), inst_scope, *(new TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols)));

//	decl -> build_scope();

//	decl -> define();
//	decl -> check();
//	decl -> gen().gen();

    return decl;
}

const Symbol* Compiler::getSpec(const TemplateStructSymbol *sym, std::vector<TemplateParam> tmpl_params, Scope *scope)
{
    return getSpecDecl(sym, tmpl_params, scope) -> getDefinedSymbol();
}
*/
