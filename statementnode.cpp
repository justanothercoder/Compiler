#include "statementnode.hpp"

StatementNode::StatementNode(std::vector<ASTNode> statements) : statements_(std::move(statements)) { }

ASTNode StatementNode::copyTree() const
{
    auto stats = std::vector<ASTNode>{ };

    for ( const auto& statement : statements_ )
        stats.push_back(statement -> copyTree());

    return std::make_unique<StatementNode>(std::move(stats));
}

ASTChildren StatementNode::getChildren() const 
{
    auto children = ASTChildren{ };
    for ( const auto& stat : statements_ )
        children.push_back(stat.get());

    return children;
}

std::string StatementNode::toString() const
{
    auto res = std::string("{\n");

    for ( const auto& statement : statements_ )
        res += statement -> toString() + '\n';

    res += "}";

    return res;
}

void StatementNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
const std::vector<ASTNode>& StatementNode::statements() const { return statements_; }
