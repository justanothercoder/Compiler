#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "templateinfo.hpp"

#include "variablenode.hpp"

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

Symbol* TemplateStructSymbol::getSpec(vector<ExprNode*> symbols) const
{
	for ( auto i : symbols )
	{
		i -> scope = holder -> scope;
		i -> template_info = new TemplateInfo();
		i -> build_scope();	
	}

	for ( auto i : symbols )
		i -> check();

	auto hash_func = [](vector<ExprNode*> vec)
	{
		unsigned long long P = 31, pow = 1, ans = 0;

		for ( size_t i = 0; i < vec.size(); ++i )
		{
//			ans += ((long long)vec[i]) * pow;
			ans += (/*(long long)*/static_cast<int>(vec[i] -> getCompileTimeValue())) * pow;
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

	StructDeclarationNode *decl = new StructDeclarationNode(this -> getName() + std::to_string(hash_), vec);

	decl -> scope = holder -> scope;
	decl -> template_info = new TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols);
	decl -> build_scope();

	decl -> define();
	decl -> check();
	decl -> gen().gen();

//	return (specs[symbols] = static_cast<StructSymbol*>(decl -> getDefinedSymbol()));
	return (specs[hash_] = static_cast<StructSymbol*>(decl -> getDefinedSymbol()));
}
