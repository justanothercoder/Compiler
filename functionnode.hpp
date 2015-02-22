#ifndef _FUNCTIONNODE_HPP_
#define _FUNCTIONNODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctionsymbol.hpp"

class FunctionNode : public ExprNode
{
public:

    FunctionNode(const std::string& name);
    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;

    const FunctionalSymbol* function();    
    void function(const FunctionalSymbol* sym);

private:

    std::string name_;
    const FunctionalSymbol* function_ = nullptr;
};

#endif
