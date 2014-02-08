#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"

map< vector<ExprNode*>, StructSymbol*> TemplateStructSymbol::specs;

TemplateStructSymbol::TemplateStructSymbol(string name, Scope *enclosing_scope, const vector< pair<string, Type*> >& template_symbols, TemplateDeclHolder *holder) : 
    StructSymbol(name, enclosing_scope),
    template_symbols(template_symbols),
    holder(holder)
{
    
}

SymbolType TemplateStructSymbol::getSymbolType() const { return SymbolType::TEMPLATESTRUCT; }

bool TemplateStructSymbol::isVarIn(string name) const 
{ 
    return std::find_if(std::begin(template_symbols),
			std::end(template_symbols),
			[&](const pair<string, Type*>& p){ return name == p.first; }
	) != std::end(template_symbols);
}

bool TemplateStructSymbol::isClassIn(string name) const 
{    
    return std::find(std::begin(template_classes), 
		     std::end(template_classes), 
		     name) != std::end(template_classes); 
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

ExprNode* TemplateStructSymbol::getReplacementForClass(string name, const vector<ExprNode*>& expr) const
{
    for ( unsigned int i = 0; i < template_classes.size(); ++i )
    {
	if ( template_classes[i] == name )
	    return expr[i];
    }
    return nullptr;    
}

Symbol* TemplateStructSymbol::getSpec(const vector<ExprNode*>& symbols) const
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
    
    vector<DeclarationNode*> vec;
	   
    for ( auto t : children )
	vec.push_back(static_cast<DeclarationNode*>(t->copyTree()));
    
    StructDeclarationNode *decl = new StructDeclarationNode(this->getName() + std::to_string(hash_func(symbols)), vec);

    for ( auto i : symbols )
    {
	i->setScope(holder->getDeclScope());
	i->build_scope();	
    }

    for ( auto i : symbols )
	i->check();	
    
    decl->setScope(holder->getDeclScope());
    decl->build_scope();
    
    decl->template_define(this, symbols);
    decl->template_check(this, symbols);    
    decl->template_gen(this, symbols);
    
    return (specs[symbols] = static_cast<StructSymbol*>(decl->getDefinedSymbol()));
}
