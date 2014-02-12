#include "templatestructdeclarationnode.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(string name, const vector<DeclarationNode*>& inner, const vector< pair<string, TypeInfo> >& template_params) : StructDeclarationNode(name, inner), template_params(template_params) { }

void TemplateStructDeclarationNode::build_scope()
{
    definedSymbol = new TemplateStructSymbol(name, this->getScope(), { }, this);

    for ( auto decl : inner )
    {
		decl->setScope(definedSymbol);
		decl->build_scope();
    }
}

void TemplateStructDeclarationNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    this->getScope()->accept(new SymbolDefine(definedSymbol)); 
    static_cast<TemplateStructSymbol*>(definedSymbol)->template_symbols = template_params;
}

void TemplateStructDeclarationNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }
void TemplateStructDeclarationNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }
	
Scope* TemplateStructDeclarationNode::getDeclScope() const { return this->getScope(); }

vector<AST*> TemplateStructDeclarationNode::getChildren() const
{ 
	vector<AST*> res(inner.size());
	std::transform(std::begin(inner), std::end(inner), std::begin(res), [](DeclarationNode *decl) { return static_cast<AST*>(decl); });

	return res; 
}
