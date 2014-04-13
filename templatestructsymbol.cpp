#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"

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
	auto it = specs.find(symbols);
	if ( it != std::end(specs) )
		return it->second;

	auto hash_func = [](vector<ExprNode*> vec)
	{
		long long P = 31, pow = 1, ans = 0;

		for ( size_t i = 0; i < vec.size(); ++i )
		{
			ans += ((long long)vec[i]) * pow;
			pow *= P;		
		}

		return ans;
	};

	auto children = holder->getChildren();

	vector<AST*> vec;

	for ( auto t : children )
		vec.push_back(t->copyTree());

	StructDeclarationNode *decl = new StructDeclarationNode(this->getName() + std::to_string(hash_func(symbols)), vec);

	for ( auto i : symbols )
	{
		i->setScope(holder->getScope());
		i->build_scope();	
	}

	for ( auto i : symbols )
		i->check(TemplateInfo());	

	decl->setScope(holder->getScope());
	decl->build_scope();

	decl->define(TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols));
	decl->check(TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols));
	decl->gen(TemplateInfo(const_cast<TemplateStructSymbol*>(this), symbols)).gen();

	return (specs[symbols] = static_cast<StructSymbol*>(decl->getDefinedSymbol()));
}
