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

const vector<DeclarationNode*>& StructDeclarationNode::getInner() const { return inner; }

AST* StructDeclarationNode::copyTree() const
{
    vector<DeclarationNode*> in;

    std::transform(std::begin(inner), std::end(inner), std::back_inserter(in), [&](DeclarationNode *decl) { return static_cast<DeclarationNode*>(decl->copyTree()); });
    
    return new StructDeclarationNode(name, in);
}

void StructDeclarationNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    for ( auto decl : inner )
		decl->gen(template_sym, expr);
}
