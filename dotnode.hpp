#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"

class Symbol;
class ObjectType;
class VariableSymbol;

class DotNode : public ExprNode
{
public:

    DotNode(ASTExprNode base, const std::string& member_name);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    ExprNode* base();
    
    const Symbol* member() const;
    void member(const Symbol* sym);

    const std::string& memberName() const;

    const ObjectType* baseType() const;
    void baseType(const ObjectType* tp);

private:

    ASTExprNode base_;
    std::string member_name;

    const ObjectType* base_type     = nullptr;
    const Symbol* member_ = nullptr;
};

#endif
