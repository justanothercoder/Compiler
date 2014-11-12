#include "unsafeblocknode.hpp"
#include "unsafescope.hpp"

UnsafeBlockNode::UnsafeBlockNode(StatementNode *block) : block(block)
{

}

AST* UnsafeBlockNode::copyTree() const
{
    return new UnsafeBlockNode(static_cast<StatementNode*>(block -> copyTree()));
}

void UnsafeBlockNode::build_scope()
{
    block -> scope = new UnsafeScope(scope);
    block -> build_scope();
}

std::vector<AST*> UnsafeBlockNode::getChildren() const
{
    return block -> getChildren();
}

std::string UnsafeBlockNode::toString() const
{
    return "unsafe\n" + block -> toString();
}

void UnsafeBlockNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}