#ifndef _FUNCTIONNODE_HPP_
#define _FUNCTIONNODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctionsymbol.hpp"

class FunctionNode : public ExprNode
{

    friend class TemplateStructSymbol;
    friend class Scope;

    friend class GenSSAVisitor;
    friend class CheckVisitor;
    friend class IsDefinedVisitor;

public:

    FunctionNode(std::string name);

    AST* copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    std::string name;
    OverloadedFunctionSymbol *function;
};

#endif
