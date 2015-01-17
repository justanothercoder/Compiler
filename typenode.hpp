#ifndef _TYPENODE_HPP_
#define _TYPENODE_HPP_

#include "exprnode.hpp"

class Symbol;

class TypeNode : public ExprNode
{
public:

    TypeNode(const std::string& name);

    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;

    const Symbol* typeSymbol() const;
    void typeSymbol(const Symbol* sym);

private:

    std::string name_;
    const Symbol* type_symbol = nullptr;
};

#endif
