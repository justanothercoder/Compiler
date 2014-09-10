#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "templateinfo.hpp"

#include "variablenode.hpp"

TemplateStructSymbol::TemplateStructSymbol(std::string name
		                                 , Scope *enclosing_scope
										 , std::vector< std::pair<std::string, TypeInfo> > template_symbols
										 , AST *holder) : StructSymbol(name, enclosing_scope, (*new TemplateInfo()))
														, template_symbols(template_symbols)
														, holder(holder)
{

}

SymbolType TemplateStructSymbol::getSymbolType() const 
{
   	return SymbolType::TEMPLATESTRUCT; 
}

bool TemplateStructSymbol::isIn(std::string name) const 
{ 
	return std::find_if(std::begin(template_symbols),
			std::end(template_symbols),
			[&](std::pair<std::string, TypeInfo> p){ return name == p.first; }
			) != std::end(template_symbols);
}

Symbol* TemplateStructSymbol::getSpec(std::vector<TemplateParam> symbols) const
{
	auto hash_func = [](std::vector<TemplateParam> vec)
	{
		unsigned long long P = 31, pow = 1, ans = 0;

		for ( size_t i = 0; i < vec.size(); ++i )
		{
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

	std::vector<AST*> vec;

	for ( auto t : children )
		vec.push_back(t -> copyTree());

	StructDeclarationNode *decl = new StructDeclarationNode(this -> getName() + "~hash" + std::to_string(hash_), vec);

	decl -> scope = new StructSymbol(getName(), holder -> scope, *(new TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols)));
	decl -> build_scope();

	decl -> define();
	decl -> check();
	decl -> gen().gen();

	return (specs[hash_] = static_cast<StructSymbol*>(decl -> getDefinedSymbol()));
}
