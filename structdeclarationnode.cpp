#include "structdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "symboldefine.hpp"
#include "functionsymbol.hpp"

StructDeclarationNode::StructDeclarationNode(std::string name, std::vector<AST*> inner, boost::optional<TemplateInfo> template_info) 
																	: name(name)
																	, inner(inner)
																	, template_info(template_info)
																	, definedSymbol(nullptr)
{

}

Symbol* StructDeclarationNode::getDefinedSymbol() const 
{ 
	return definedSymbol; 
}

void StructDeclarationNode::build_scope()
{
	if ( !template_info )
	    definedSymbol = new StructSymbol(name, scope, scope -> getTemplateInfo());
	else
	    definedSymbol = new StructSymbol(name, scope, *template_info);

	scope -> define(definedSymbol);

	for ( auto i : inner )
    {
		i -> scope = definedSymbol;
		i -> build_scope();
    }
}

AST* StructDeclarationNode::copyTree() const 
{ 
	std::vector<AST*> in(inner.size());
	std::transform(std::begin(inner), std::end(inner), std::begin(in), [&](AST *t) { return t -> copyTree(); });
	return new StructDeclarationNode(name, in, template_info);
}

std::vector<AST*> StructDeclarationNode::getChildren() const 
{
   	return inner; 
}	

std::string StructDeclarationNode::toString() const
{
	std::string res = "struct " + name + "\n{\n";
	
	for ( auto decl : inner )
		res += decl -> toString() + '\n';

	res += "}";
	return res;
}

void StructDeclarationNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
