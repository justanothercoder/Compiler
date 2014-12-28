#ifndef _TEMPLATEFUNCTIONNODE_HPP_
#define _TEMPLATEFUNCTIONNODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "typeinfo.hpp"

class TemplateFunctionNode : public ExprNode
{

    friend class TemplateStructSymbol;
    friend class Scope;

    friend class MarkReturnAsInlineVisitor;
    friend class GenSSAVisitor;
    friend class CheckVisitor;

public:

    TemplateFunctionNode(std::string name, std::vector<TemplateParamInfo> template_params);

    AST* copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;


    std::string name;    
    OverloadedFunctionSymbol *function;
    std::vector<TemplateParamInfo> template_params;
};

#endif
