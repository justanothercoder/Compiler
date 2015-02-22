#ifndef _IMPORTNODE_HPP_
#define _IMPORTNODE_HPP_

#include "ast.hpp"

class Symbol;

class ImportNode : public AST
{
public:
    ImportNode(std::string lib, AST* root, std::vector<const Symbol*> imports);

    void build_scope();        

    ASTNode copyTree() const override;
    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    std::string lib;
    AST* root;
    std::vector<const Symbol*> imports;
};

#endif
