#include "unsafeblocknode.hpp"
#include "unsafescope.hpp"

UnsafeBlockNode::UnsafeBlockNode(ASTNode block) : block_(std::move(block)) { }

ASTNode UnsafeBlockNode::copyTree() const { return std::make_unique<UnsafeBlockNode>(block_ -> copyTree()); }

void UnsafeBlockNode::build_scope()
{
    block_ -> scope = std::make_shared<UnsafeScope>(scope.get());
    block_ -> build_scope();
}

ASTChildren UnsafeBlockNode::getChildren() const { return block_ -> getChildren(); }
std::string UnsafeBlockNode::toString() const { return "unsafe\n" + block_ -> toString(); }

void UnsafeBlockNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
AST* UnsafeBlockNode::block() { return block_.get(); }
