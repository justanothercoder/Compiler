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
    definedSymbol = new StructSymbol(name, this->getScope());
    for ( auto i : inner )
    {
		i->setScope(definedSymbol);
		i->build_scope();
    }
}

void StructDeclarationNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    this->getScope()->accept(new SymbolDefine(definedSymbol));

    for ( auto decl : inner )
		decl->define(template_sym, expr);
}

void StructDeclarationNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    for ( auto decl : inner )
		decl->check(template_sym, expr);	
}

void StructDeclarationNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    for ( auto decl : inner )
		decl->gen(template_sym, expr);
}

AST* StructDeclarationNode::copyTree() const 
{ 
	auto in = inner;
	return new StructDeclarationNode(name, in); 
}

vector<AST*> StructDeclarationNode::getChildren() const { return inner; } 
