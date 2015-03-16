#ifndef _FUNCTIONNODE_HPP_
#define _FUNCTIONNODE_HPP_

#include "exprnode.hpp"
#include "modulesymbol.hpp"

class FunctionalSymbol;

class FunctionNode : public ExprNode
{
public:

    FunctionNode(const std::string& name, ModuleSymbol* sym = nullptr);
    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;

    FunctionalSymbol* function();    
    void function(FunctionalSymbol* sym);

    ModuleSymbol* module() const;

private:

    std::string name_;
    FunctionalSymbol* function_ = nullptr;

    ModuleSymbol* module_ = nullptr;
};

#endif
