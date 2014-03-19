#include "templatestructdeclarationnode.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(string name, const vector<AST*>& inner, const vector< pair<string, TypeInfo> >& template_params) : StructDeclarationNode(name, inner), template_params(template_params) { }

void TemplateStructDeclarationNode::build_scope()
{
    definedSymbol = new TemplateStructSymbol(name, getScope(), template_params, this);

    for ( auto decl : inner )
    {
		decl->setScope(definedSymbol);
		decl->build_scope();
    }
}

void TemplateStructDeclarationNode::define(const TemplateInfo&) { getScope()->accept(new SymbolDefine(definedSymbol)); }

void TemplateStructDeclarationNode::gen(const TemplateInfo&) { }
void TemplateStructDeclarationNode::check(const TemplateInfo&) { }
