#include "importnode.hpp"
#include "scope.hpp"
#include "symbol.hpp"

ImportNode::ImportNode(const std::string& lib
                     , std::shared_ptr<AST> root
                     , std::vector< std::shared_ptr<const Symbol> > imports) : lib(lib)
                                                                             , root(root)
                                                                             , imports(imports) 
{

}

void ImportNode::build_scope()
{
    root -> scope = scope;

    for ( auto import : imports )
        scope -> define(import);
}

ASTNode ImportNode::copyTree() const { return std::make_unique<ImportNode>(lib, root -> copyTree(), imports); }

std::string ImportNode::toString() const { return "import " + lib + ";"; }

void ImportNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
