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

ExprNode* TemplateStructSymbol::getReplacement(string name, const vector<ExprNode*>& expr) const
{
	for ( unsigned int i = 0; i < template_symbols.size(); ++i )
	{
		if ( template_symbols[i].first == name )
			return expr[i];
	}
	return nullptr;
}

Symbol* TemplateStructSymbol::getSpec(vector<ExprNode*> symbols) const
{
	auto it = specs.find(symbols);
	if ( it != std::end(specs) )
		return it->second;

	auto hash_func = [](const vector<ExprNode*>& vec)
	{
		long long P = 31, pow = 1, ans = 0;

		for ( int i = 0; i < static_cast<int>(vec.size()); ++i )
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
		i->check(nullptr, { });	

	decl->setScope(holder->getScope());
	decl->build_scope();

	decl->define(this, symbols);
	decl->check(this, symbols);    
	decl->gen(this, symbols);

	return (specs[symbols] = static_cast<StructSymbol*>(decl->getDefinedSymbol()));
}
