#ifndef _BREAKNODE_HPP_
#define _BREAKNODE_HPP_

#include "ast.hpp"

class BreakNode : public AST
{
public:

    BreakNode();
    
    AST* copyTree() const override;
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;
};

#endif
