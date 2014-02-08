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
    this->getScope()->accept(new SymbolDefine(definedSymbol));
    
    static_cast<TemplateStructSymbol*>(definedSymbol)->template_symbols = template_params;
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
