#include "importnode.hpp"

ImportNode::ImportNode(std::string lib, AST* root) : lib(lib), root(root)
{

}

void ImportNode::build_scope()
{
    root -> scope = scope;
    root -> build_scope();
}

AST* ImportNode::copyTree() const
{
    return new ImportNode(lib, root -> copyTree());
}

std::string ImportNode::toString() const
{
    return "import " + lib + ";";
}

void ImportNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
