#include "structdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "symboldefine.hpp"
#include "functionsymbol.hpp"

StructDeclarationNode::StructDeclarationNode(const std::string& name
                                           , std::vector<ASTNode> inner
                                           , boost::optional<TemplateInfo> template_info) : template_info(template_info)
                                                                                          , name_(name)
                                                                                          , inner_(std::move(inner))
{

}

const Symbol* StructDeclarationNode::getDefinedSymbol() const { return defined_symbol.get(); }

void StructDeclarationNode::build_scope()
{
    defined_symbol = std::make_shared<StructSymbol>(name_, scope.get(), (!template_info ? scope -> templateInfo() : *template_info));
    scope -> define(defined_symbol);

    for ( const auto& decl : inner_ )
    {
        decl -> scope = defined_symbol;
        decl -> build_scope();
    }
}

ASTNode StructDeclarationNode::copyTree() const
{
    auto in = std::vector<ASTNode>{ };
    
    for ( const auto& decl : inner_ )
        in.push_back(decl -> copyTree());
    
    return std::make_unique<StructDeclarationNode>(name_, std::move(in), template_info);
}

ASTChildren StructDeclarationNode::getChildren() const 
{
    auto in = ASTChildren{ };
    
    for ( const auto& decl : inner_ )
        in.push_back(decl.get());
    
    return in;
}    

std::string StructDeclarationNode::toString() const
{
    auto res = "struct " + name_ + "\n{\n";

    for ( const auto& decl : inner_ )
        res += decl -> toString() + '\n';

    res += "}";
    return res;
}

void StructDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
