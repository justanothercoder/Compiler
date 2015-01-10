#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"

class Symbol;
class ObjectType;
class VariableSymbol;

class DotNode : public ExprNode
{

    friend class GenSSAVisitor;
    friend class CheckVisitor;
    friend class ExpandTemplatesVisitor;

public:

    DotNode(ExprNode *base, std::string member_name);

    std::vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    ExprNode *base;

    std::string member_name;

    const ObjectType* base_type;
    Symbol* member;
};

#endif
