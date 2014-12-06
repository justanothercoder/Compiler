#ifndef _STRINGNODE_HPP_
#define _STRINGNODE_HPP_

#include "exprnode.hpp"

class StringNode : public ExprNode
{
public:

    StringNode(std::string str);

    AST* copyTree() const override;

    std::string getStr() const;

    VariableType getType() const override;
    bool isLeftValue() const override;

    bool isCompileTimeExpr() const override;
    boost::optional<int> getCompileTimeValue() const override;

    std::string toString() const override;

    void accept(ASTVisitor& visitor) override;

    std::string str;
};

#endif
