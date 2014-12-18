#include "importnode.hpp"
#include "scope.hpp"

ImportNode::ImportNode(std::string lib, AST* root, std::vector<Symbol*> imports) : lib(lib), root(root), imports(imports)
{

}

void ImportNode::build_scope()
{
    root -> scope = scope;

    for ( auto import : imports )
        scope -> define(import);
}

AST* ImportNode::copyTree() const
{
    return new ImportNode(lib, root -> copyTree(), imports);
}

std::string ImportNode::toString() const
{
    return "import " + lib + ";";
}

void ImportNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
