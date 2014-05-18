#include "templatestructdeclarationnode.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(string name, const vector<AST*>& inner, const vector< pair<string, TypeInfo> >& template_params) : StructDeclarationNode(name, inner), template_params(template_params) { }

void TemplateStructDeclarationNode::build_scope()
{
    definedSymbol = new TemplateStructSymbol(name, scope, template_params, this);

    for ( auto decl : inner )
    {
		decl->scope = definedSymbol;
		decl->build_scope();
    }
}

void TemplateStructDeclarationNode::define(const TemplateInfo&) { scope->accept(new SymbolDefine(definedSymbol)); }

void TemplateStructDeclarationNode::check(const TemplateInfo&) { }
CodeObject& TemplateStructDeclarationNode::gen(const TemplateInfo&) { return code_obj; }
