#include "structdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"

StructDeclarationNode::StructDeclarationNode(const std::string& name
                                           , std::vector<ASTNode> inner) : name_(name)
                                                                         , inner_(std::move(inner))
{

}

Symbol* StructDeclarationNode::getDefinedSymbol() const { return defined_symbol; }

void StructDeclarationNode::build_scope()
{
    struct_scope = std::make_shared<StructScope>(name_, scope.get());

    for ( const auto& decl : inner_ )
    {
        decl -> scope = struct_scope;
        decl -> build_scope();
    }
}

ASTNode StructDeclarationNode::copyTree() const
{
    auto in = std::vector<ASTNode>{ };
    
    for ( const auto& decl : inner_ )
        in.push_back(decl -> copyTree());
    
    return std::make_unique<StructDeclarationNode>(name_, std::move(in));
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
    
const std::string& StructDeclarationNode::name() const { return name_; }
const std::vector<ASTNode>& StructDeclarationNode::inner() const { return inner_; }
    
StructScope* StructDeclarationNode::structScope() const { return struct_scope.get(); }
