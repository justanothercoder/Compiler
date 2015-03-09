#ifndef _LAMBDANODE_HPP_
#define _LAMBDANODE_HPP_

#include <vector>
#include <string>
#include "exprnode.hpp"
#include "paraminfo.hpp"

class FunctionalSymbol;

class LambdaNode : public ExprNode
{
public:

    LambdaNode(std::vector<std::string> capture, std::vector<ParamInfo> formal_params, ASTNode body);

    void build_scope() override;

    ASTNode copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    const std::vector<std::string>& capture() const;
    const std::vector<ParamInfo> formalParams() const;
    AST* body() const;

    void setLambdaType(Type* t);
    const FunctionalSymbol* callOp() const;
    const FunctionalSymbol* constructor() const;

private:

    std::vector<std::string> capture_;
    std::vector<ParamInfo> formal_params;
    ASTNode body_;

    Type* lambda_inner_type = nullptr;
};

#endif
