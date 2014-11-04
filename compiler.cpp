#include "compiler.hpp"
#include "scope.hpp"
#include "structdeclarationnode.hpp"
#include "typefactory.hpp"
#include "exprnode.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"


const Type* Compiler::fromTypeInfo(const TypeInfo& type_info, Scope *scope)
{
    auto type_name = type_info.type_name;

    const Type *type = scope -> resolveType(type_name);

    if ( type_info.pointer_depth > 0 && !scope -> isUnsafeBlock() )
        throw SemanticError("Using pointer type in safe block " + scope -> getScopeName() + ".");
    
//    if ( type_info.array_dimensions.size() > 0 && !scope -> isUnsafeBlock() )
//        throw SemanticError("Using plain array type in safe block " + scope -> getScopeName() + ".");

    if ( type == nullptr )
        throw SemanticError(type_name + " is not a type.");
    
    for ( int i = 0; i < type_info.pointer_depth; ++i )
        type = TypeFactory::getPointer(type);

    for ( auto dim : type_info.array_dimensions )
    {
        if ( !dim -> isCompileTimeExpr() )
            throw SemanticError("Array dimension is not compile-time expression.");
        type = TypeFactory::getArray(type, *dim -> getCompileTimeValue());
    }
    
    if ( type_info.is_ref )
        type = TypeFactory::getReference(type);

    if ( type_info.is_const )
        type = TypeFactory::getConst(type);

    return type;

}

DeclarationNode* Compiler::getSpecDecl(const TemplateStructSymbol *sym, std::vector<TemplateParam> template_params)
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

	auto it = sym -> specs.find(hash_);
	if ( it != std::end(sym -> specs) )
		return it -> second;

	auto children = sym -> holder -> getChildren();

	std::vector<AST*> vec;

	for ( auto t : children )
		vec.push_back(t -> copyTree());

    auto decl = new StructDeclarationNode(sym -> getName() + "~hash" + std::to_string(hash_), 
                                          vec, 
                                          *(new TemplateInfo(const_cast<TemplateStructSymbol*>(sym), 
                                                             template_params)
                                           )
    );


	decl -> scope = sym -> holder -> scope;
    decl -> build_scope();

    return (sym -> specs[hash_] = decl);
}

const Symbol* Compiler::getSpec(const TemplateStructSymbol *sym, std::vector<TemplateParam> tmpl_params)
{
    return getSpecDecl(sym, tmpl_params) -> getDefinedSymbol();
}
