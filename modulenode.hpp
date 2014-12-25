#ifndef _MODULENODE_HPP_
#define _MODULENODE_HPP_

#include "exprnode.hpp"

class ModuleSymbol;

class ModuleNode : public ExprNode
{

    friend class TemplateStructSymbol;
    friend class Scope;

    friend class GenSSAVisitor;
    friend class CheckVisitor;

public:

    ModuleNode(std::string name);

    AST* copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    std::string name;
    ModuleSymbol *module;
};

#endif
