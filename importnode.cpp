#include "importnode.hpp"
#include "scope.hpp"
#include "symbol.hpp"
#include "aliassymbol.hpp"

ImportNode::ImportNode(std::string lib, AST* root, std::vector<const Symbol*> imports) : lib(lib), root(root), imports(imports) { }

void ImportNode::build_scope()
{
    root -> scope = scope;

    for ( auto import : imports )
        scope -> define(std::make_unique<AliasSymbol>(import));
}

ASTNode ImportNode::copyTree() const { return std::make_unique<ImportNode>(lib, root, imports); }

std::string ImportNode::toString() const { return "import " + lib + ";"; }

void ImportNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
