#include "templatestructdeclarationnode.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(string name, const vector<DeclarationNode*>& inner, const vector< pair<string, TypeInfo> >& template_params) : StructDeclarationNode(name, inner), template_params(template_params)
{
    
}

void TemplateStructDeclarationNode::build_scope()
{
    definedSymbol = new TemplateStructSymbol(name, this->getScope(), { }, this);

    for ( auto decl : inner )
    {
	decl->setScope(definedSymbol);
	decl->build_scope();
    }
}

void TemplateStructDeclarationNode::define()
{
    this->getScope()->define(definedSymbol);

    vector< pair<string, Type*> > template_symbols;
    vector<string> template_classes;
    
    for ( auto p : template_params )
    {
	if ( p.second.getTypeName() == "class" )
	{
	    template_classes.push_back(p.first);	    
	}
	else
	{
	    auto type = TypeHelper::fromTypeInfo(p.second, this->getScope());
	    template_symbols.push_back({p.first, type});	    
	}
    }

    static_cast<TemplateStructSymbol*>(definedSymbol)->template_symbols = template_symbols;
    static_cast<TemplateStructSymbol*>(definedSymbol)->template_classes = template_classes;

//    for ( auto i : inner )
//	i->define();
}

void TemplateStructDeclarationNode::check() { }

void TemplateStructDeclarationNode::gen()
{
    
}

void TemplateStructDeclarationNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto i : inner )
	i->template_check(template_sym, expr);
}

vector<AST*> TemplateStructDeclarationNode::getChildren() const
{
    vector<AST*> res;

    std::transform(std::begin(inner), std::end(inner), std::back_inserter(res), [](DeclarationNode* d) { return static_cast<AST*>(d); });
    
    return res;
}

Scope* TemplateStructDeclarationNode::getDeclScope() const { return this->getScope(); }
