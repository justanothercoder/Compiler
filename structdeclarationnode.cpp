#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<AST*>& inner) : name(name), inner(inner), definedSymbol(nullptr) { }

StructDeclarationNode::~StructDeclarationNode() 
{ 
	delete definedSymbol; 

	for ( auto i : inner )
		delete i;
}

Symbol* StructDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void StructDeclarationNode::build_scope()
{
    definedSymbol = new StructSymbol(name, getScope());
    getScope()->accept(new SymbolDefine(definedSymbol));
    for ( auto i : inner )
    {
		i->setScope(definedSymbol);
		i->build_scope();
    }
}

void StructDeclarationNode::define(const TemplateInfo& template_info)
{
    for ( auto decl : inner )
		decl->define(template_info);
}

void StructDeclarationNode::check(const TemplateInfo& template_info)
{
	GlobalHelper::setDefined(getDefinedSymbol());

	for ( auto decl : inner )
	{
		if ( dynamic_cast<DeclarationNode*>(decl) )
			GlobalHelper::setDefined(static_cast<DeclarationNode*>(decl)->getDefinedSymbol());
	}

	for ( auto decl : inner )
		decl->check(template_info);	
}

void StructDeclarationNode::gen(const TemplateInfo& template_info)
{
    for ( auto decl : inner )
		decl->gen(template_info);
}

AST* StructDeclarationNode::copyTree() const 
{ 
	vector<AST*> in(inner.size());
	std::transform(std::begin(inner), std::end(inner), std::begin(in), [&](AST *t) { return t->copyTree(); });
	return new StructDeclarationNode(name, in);
}

vector<AST*> StructDeclarationNode::getChildren() const { return inner; } 
