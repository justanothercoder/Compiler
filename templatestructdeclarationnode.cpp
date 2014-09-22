#include "templatestructdeclarationnode.hpp"
#include "symboldefine.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(std::string name
		                                                   , std::vector<AST*> inner
														   , std::vector< std::pair<std::string, TypeInfo> > template_params) 
																											: StructDeclarationNode(name, inner)
																											, template_params(template_params) 
{

}

void TemplateStructDeclarationNode::build_scope()
{
    definedSymbol = new TemplateStructSymbol(name, scope, template_params, this);

    for ( auto decl : inner )
    {
		decl -> scope = definedSymbol;
		decl -> build_scope();
    }
}

void TemplateStructDeclarationNode::define() 
{
   	scope -> accept(*(new SymbolDefine(definedSymbol)));
}

void TemplateStructDeclarationNode::check() 
{

}

CodeObject& TemplateStructDeclarationNode::gen() 
{ 
	return code_obj; 
}
