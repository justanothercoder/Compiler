#ifndef _STATEMENTNODE_HPP_
#define _STATEMENTNODE_HPP_

#include <vector>
#include <algorithm>

#include "ast.hpp"

class StatementNode : public AST
{
public:

    StatementNode(std::vector<ASTNode> statements);
    void accept(ASTVisitor& visitor) override;

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    std::string toString() const override;

    const std::vector<ASTNode>& statements() const;

private:    
    std::vector<ASTNode> statements_;
};

#endif
