#ifndef _MODULENODE_HPP_
#define _MODULENODE_HPP_

#include "exprnode.hpp"

class ModuleSymbol;

class ModuleNode : public ExprNode
{
public:

    ModuleNode(std::string name);

    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;
    
    const ModuleSymbol* module() const;
    void module(const ModuleSymbol* module);

private:

    std::string name_;
    const ModuleSymbol* module_ = nullptr;
};

#endif
