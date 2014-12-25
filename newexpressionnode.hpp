#ifndef _NEWEXPRESSIONNODE_HPP_
#define _NEWEXPRESSIONNODE_HPP_

#include <vector>
#include <string>
#include <algorithm>

#include "exprnode.hpp"
#include "typeinfo.hpp"
#include "callinfo.hpp"

class NewExpressionNode : public ExprNode
{

    friend class GenSSAVisitor;
    friend class CheckVisitor;
    friend class ExpandTemplatesVisitor;

public:
    NewExpressionNode(TypeInfo type_info, std::vector<ExprNode*> params);

    void build_scope() override;

    std::vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

private:

    TypeInfo type_info;
    std::vector<ExprNode*> params;
    CallInfo call_info;
};

#endif
