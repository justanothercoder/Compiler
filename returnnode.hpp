#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "callinfo.hpp"
#include "ast.hpp"

class ExprNode;
class FunctionSymbol;

class ReturnNode : public AST
{
public:

    ReturnNode(ExprNode *expr);

    AST* copyTree() const override;

    std::vector<AST*> getChildren() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    ExprNode *expr;

    CallInfo copy_call_info;

    FunctionSymbol *enclosing_func;
};

#endif
