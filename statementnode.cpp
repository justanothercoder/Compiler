#include "statementnode.hpp"

StatementNode::StatementNode(std::vector<AST*> statements) : statements(statements)
{

}

AST* StatementNode::copyTree() const
{
    std::vector<AST*> stats(statements.size());

    std::transform(std::begin(statements), std::end(statements), std::begin(stats), [&](AST *t)
    {
        return t -> copyTree();
    });

    return new StatementNode(stats);
}

std::vector<AST*> StatementNode::getChildren() const
{
    return statements;
}

std::string StatementNode::toString() const
{
    std::string res = "{\n";

    for ( auto statement : statements )
        res += statement -> toString() + '\n';

    res += "}";

    return res;
}

void StatementNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
