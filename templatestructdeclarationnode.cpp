#include "templatestructdeclarationnode.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(string name,
							     const vector<DeclarationNode*>& inner,
							     const vector< pair<string, TypeInfo> >& template_params) : StructDeclarationNode(name, inner), template_params(template_params)
{
    
}

void TemplateStructDeclarationNode::build_scope()
{
    definedSymbol = new TemplateStructSymbol(name, this->getScope(), { });

    std::for_each(std::begin(inner), std::end(inner), [&](DeclarationNode *decl) { decl->setScope(definedSymbol); decl->build_scope(); });
}

void TemplateStructDeclarationNode::define()
{
    this->getScope()->define(definedSymbol);

    vector<Symbol*> template_symbols;
    
    for ( auto p : template_params )
    {
	if ( p.second.getTypeName() == "class" )
	{
	    auto sym = new StructSymbol(p.first, definedSymbol);
	    template_symbols.push_back(sym);
	    definedSymbol->define(sym);
	}
	else
	{
	    auto type = TypeHelper::fromTypeInfo(p.second, this->getScope());

	    auto sym = new VariableSymbol(p.first, type);
	    template_symbols.push_back(sym);
	    
	    definedSymbol->define(sym);
	}
    }

    static_cast<TemplateStructSymbol*>(definedSymbol)->template_symbols = template_symbols;

    for ( auto i : inner )
	i->define();
}

void TemplateStructDeclarationNode::check()
{

}

void TemplateStructDeclarationNode::template_check(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto i : inner )
	i->template_check(template_sym, expr);
}
