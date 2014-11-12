#include "importnode.hpp"
#include "filehelper.hpp"

ImportNode::ImportNode(std::string lib) : lib(lib), root(nullptr)
{

}

void ImportNode::build_scope()
{
    root = FileHelper::parse((lib + ".txt").c_str());
    
    root -> scope = scope;
    root -> build_scope();
}

AST* ImportNode::copyTree() const
{
    return new ImportNode(lib);
}

std::string ImportNode::toString() const
{
    return "import " + lib + ";";
}

void ImportNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
