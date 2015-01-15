#ifndef _BRACKETNODE_HPP_
#define _BRACKETNODE_HPP_

#include <memory>
#include "exprnode.hpp"
#include "callinfo.hpp"

class BracketNode : public ExprNode
{
public:

    BracketNode(ASTExprNode base, ASTExprNode expr);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    ExprNode* base();
    ExprNode* expr();
    
    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

private:

    ASTExprNode base_;
    ASTExprNode expr_;

    CallInfo call_info;
};

#endif
