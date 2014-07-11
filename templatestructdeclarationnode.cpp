#include "templatestructdeclarationnode.hpp"
#include "symboldefine.hpp"
#include "templateinfo.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(string name, const vector<AST*>& inner, const vector< pair<string, TypeInfo> >& template_params) : StructDeclarationNode(name, inner), template_params(template_params) { }

void TemplateStructDeclarationNode::build_scope()
{
    definedSymbol = new TemplateStructSymbol(name, scope, template_params, this);

    for ( auto decl : inner )
    {
		decl -> scope = definedSymbol;
		decl -> template_info = new TemplateInfo();
		decl -> build_scope();
    }
}

void TemplateStructDeclarationNode::define() { scope->accept(new SymbolDefine(definedSymbol)); }

void TemplateStructDeclarationNode::check() { }
CodeObject& TemplateStructDeclarationNode::gen() { return code_obj; }
