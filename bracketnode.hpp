#ifndef _BRACKETNODE_HPP_
#define _BRACKETNODE_HPP_

#include <memory>
#include "callablenode.hpp"

class BracketNode : public CallableNode
{
public:

    BracketNode(ASTExprNode base, ASTExprNode expr);

    ASTNode copyTree() const override;
    ASTChildren getChildren() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

    ExprNode* base();
    ExprNode* expr();
    
    const CallInfo& callInfo() const;
    void callInfo(const CallInfo& call_info);

    const FunctionalType* function() const override;
    std::vector<ValueInfo> arguments() const override;

private:

    ASTExprNode base_;
    ASTExprNode expr_;
};

#endif
