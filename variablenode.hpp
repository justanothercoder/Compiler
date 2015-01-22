#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "numbernode.hpp"

class VariableSymbol;

class VariableNode : public ExprNode
{
public:

    VariableNode(const std::string& name);

    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;

    const VariableSymbol* variable() const;
    void variable(const VariableSymbol* sym);

private:

    std::string name_;
    const VariableSymbol* variable_ = nullptr;
};

#endif
