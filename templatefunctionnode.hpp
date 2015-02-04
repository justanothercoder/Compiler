#ifndef _TEMPLATEFUNCTIONNODE_HPP_
#define _TEMPLATEFUNCTIONNODE_HPP_

#include "exprnode.hpp"
#include "typeinfo.hpp"

class FunctionalType;

class TemplateFunctionNode : public ExprNode
{
public:

    TemplateFunctionNode(const std::string& name, TemplateArgumentsInfo template_arguments);

    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;

    const FunctionalType* function();
    void function(const FunctionalType* type);

    const TemplateArgumentsInfo& templateArgumentsInfo() const;

private:

    std::string name_;    
    const FunctionalType* function_ = nullptr;
    TemplateArgumentsInfo template_arguments;
};

#endif
