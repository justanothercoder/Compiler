#include "structdeclarationnode.hpp"

StructDeclarationNode::StructDeclarationNode(string name, const vector<DeclarationNode*>& inner) : name(name), inner(inner)
{
    definedSymbol = nullptr;
}

StructDeclarationNode::~StructDeclarationNode() { delete definedSymbol; }

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

void StructDeclarationNode::define()
{
    this->getScope()->accept(new SymbolDefine(definedSymbol));

    for ( auto i : inner )
	i->define();    
}

void StructDeclarationNode::check()
{
    for ( auto i : inner )
	i->check();
}

void StructDeclarationNode::gen()
{    
    for ( auto i : inner )
	i->gen();
}

void StructDeclarationNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    this->getScope()->accept(new SymbolDefine(definedSymbol));

    for ( auto decl : inner )
	decl->template_define(template_sym, expr);
}

void StructDeclarationNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto decl : inner )
	decl->template_check(template_sym, expr);	
}

const vector<DeclarationNode*>& StructDeclarationNode::getInner() const { return inner; }

AST* StructDeclarationNode::copyTree() const
{
    vector<DeclarationNode*> in;

    std::transform(std::begin(inner), std::end(inner), std::back_inserter(in), [&](DeclarationNode *decl) { return static_cast<DeclarationNode*>(decl->copyTree()); });
    
    return new StructDeclarationNode(name, in);
}

void StructDeclarationNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto decl : inner )
	decl->template_gen(template_sym, expr);
}
