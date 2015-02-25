#ifndef _MODULEMEMBERACCESS_HPP_
#define _MODULEMEMBERACCESS_HPP_

#include "exprnode.hpp"

class Symbol;

class ModuleMemberAccessNode : public ExprNode 
{
public:

    ModuleMemberAccessNode(const std::string& name, const std::string& member);

    ASTNode copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;
    
    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    const std::string& name() const;
    const std::string& member() const;

    const Symbol* memberSymbol() const;
    void memberSymbol(Symbol* sym);

private:

    std::string name_;
    std::string member_;

    Symbol* member_sym = nullptr;
};

#endif
