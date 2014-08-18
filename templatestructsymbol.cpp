#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "templateinfo.hpp"

#include "variablenode.hpp"
#include "typehelper.hpp"

TemplateStructSymbol::TemplateStructSymbol(string name, Scope *enclosing_scope, const vector< pair<string, TypeInfo> >& template_symbols, AST *holder) : 
	StructSymbol(name, enclosing_scope),
	template_symbols(template_symbols),
	holder(holder)
{

}

SymbolType TemplateStructSymbol::getSymbolType() const { return SymbolType::TEMPLATESTRUCT; }

bool TemplateStructSymbol::isIn(string name) const 
{ 
	return std::find_if(std::begin(template_symbols),
			std::end(template_symbols),
			[&](const pair<string, TypeInfo>& p){ return name == p.first; }
			) != std::end(template_symbols);
}

Symbol* TemplateStructSymbol::getSpec(std::vector<TemplateParam> symbols) const
{
	for ( auto param : symbols )
	{
		if ( param.which() == 0 ) 
			std::cerr << "Debug: " << boost::get<std::string>(param) << '\n';
		else	
			std::cerr << "Debug: " << boost::get<int>(param) << '\n';
	}
/*
	for ( auto i : symbols )
	{
		i -> scope = holder -> scope;
		i -> template_info = holder -> template_info;
		i -> build_scope();	
	}
*/
//	for ( auto i : symbols )
//		i -> check();
/*
	for ( size_t i = 0; i < template_symbols.size(); ++i )
	{
		if ( template_symbols[i].second.type_name == "class" )
		{
			if ( dynamic_cast<VariableNode*>(symbols[i]) == nullptr 
		      || dynamic_cast<ClassVariableSymbol*>(static_cast<VariableNode*>(symbols[i]) -> variable) == nullptr )
				throw SemanticError("expression is not a type");
		}
		else
		{
			if ( symbols[i] -> getType().type != TypeHelper::fromTypeInfo(template_symbols[i].second, symbols[i] -> scope, symbols[i] -> template_info).type )
				throw SemanticError("Not compatible template parameter");
		}

		if ( !symbols[i] -> isCompileTimeExpr() )
			throw SemanticError("Template parameter is not a compile-time expression");
	}
*/
	auto hash_func = [](std::vector<TemplateParam> vec)
	{
		unsigned long long P = 31, pow = 1, ans = 0;

		for ( size_t i = 0; i < vec.size(); ++i )
		{
//			ans += ((long long)vec[i]) * pow;
//			ans += (/*(long long)*/static_cast<int>(vec[i] -> getCompileTimeValue())) * pow;
			
			if ( vec[i].which() == 0 )
				ans += static_cast<int>(std::hash<std::string>()(boost::get<std::string>(vec[i])) * pow);
			else
				ans += static_cast<int>(boost::get<int>(vec[i])) * pow;

			pow *= P;		
		}

		return ans;
	};

	auto hash_ = hash_func(symbols);

	auto it = specs.find(hash_);
	if ( it != std::end(specs) )
		return it -> second;

	auto children = holder -> getChildren();

	vector<AST*> vec;

	for ( auto t : children )
		vec.push_back(t -> copyTree());

	StructDeclarationNode *decl = new StructDeclarationNode(this -> getName() + "~hash" + std::to_string(hash_), vec);

	decl -> scope = holder -> scope;
	decl -> template_info = new TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols);
	decl -> build_scope();

	decl -> define();
	decl -> check();
	decl -> gen().gen();

//	return (specs[symbols] = static_cast<StructSymbol*>(decl -> getDefinedSymbol()));
	return (specs[hash_] = static_cast<StructSymbol*>(decl -> getDefinedSymbol()));
}
