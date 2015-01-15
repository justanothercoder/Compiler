#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "callinfo.hpp"
#include "exprnode.hpp"

class FunctionSymbol;

class ReturnNode : public AST
{
    friend class MarkReturnAsInlineVisitor;
public:

    ReturnNode(ASTExprNode expr);

    ASTNode copyTree() const override; 
    ASTChildren getChildren() const override; 

    std::string toString() const override; 
    void accept(ASTVisitor& visitor) override;

    ExprNode* expr();
    bool isInInlineCall() const;

    const FunctionSymbol* function() const;
    void function(const FunctionSymbol* func);

private:

    ASTExprNode expr_; 
    const FunctionSymbol* enclosing_func = nullptr;

    bool is_in_inline_call = false;
};

#endif
