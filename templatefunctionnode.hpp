#ifndef _TEMPLATEFUNCTIONNODE_HPP_
#define _TEMPLATEFUNCTIONNODE_HPP_

#include "exprnode.hpp"
#include "typeinfo.hpp"

class ModuleSymbol;
class FunctionalSymbol;

class TemplateFunctionNode : public ExprNode
{
public:

    TemplateFunctionNode(std::string name, TemplateArgumentsInfo template_arguments, ModuleSymbol* sym = nullptr);

    ASTNode copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    const std::string& name() const;

    FunctionalSymbol* function();
    void function(FunctionalSymbol* type);

    const TemplateArgumentsInfo& templateArgumentsInfo() const;

    ModuleSymbol* module() const;

private:

    std::string name_;    
    FunctionalSymbol* function_ = nullptr;
    TemplateArgumentsInfo template_arguments;

    ModuleSymbol* module_;
};

#endif
