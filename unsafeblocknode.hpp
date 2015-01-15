#ifndef _UNSAFEBLOCKNODE_HPP_
#define _UNSAFEBLOCKNODE_HPP_

#include "ast.hpp"

class UnsafeBlockNode : public AST
{
public:

    UnsafeBlockNode(ASTNode block);
    
    void accept(ASTVisitor& visitor) override;
    void build_scope() override;
    
    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string toString() const override;

    AST* block();

private:
    ASTNode block_;
};

#endif
