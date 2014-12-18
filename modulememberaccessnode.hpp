#ifndef _MODULEMEMBERACCESS_HPP_
#define _MODULEMEMBERACCESS_HPP_

#include "exprnode.hpp"

class Symbol;

class ModuleMemberAccessNode : public ExprNode 
{

    friend class CheckVisitor;

public:

    ModuleMemberAccessNode(std::string name, std::string member);

    AST* copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;
    
    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

private:

    std::string name;
    std::string member;

    Symbol* member_sym;
};

#endif
