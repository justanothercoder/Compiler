#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "numbernode.hpp"

class VarSymbol;
class ModuleSymbol;

class VariableNode : public ExprNode
{
public:

    VariableNode(std::string name, ModuleSymbol* sym = nullptr);

    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;

    const VarSymbol* variable() const;
    void variable(const VarSymbol* sym);

    ModuleSymbol* module() const;

private:

    std::string name_;
    const VarSymbol* variable_ = nullptr;
    ModuleSymbol* module_ = nullptr;
};

#endif
