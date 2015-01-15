#ifndef _EXPRNODE_HPP_
#define _EXPRNODE_HPP_

#include <boost/optional.hpp>

#include "ast.hpp"
#include "variabletype.hpp"

class ExprNode : public AST
{
public:

    virtual VariableType getType() const = 0;
    virtual bool isLeftValue() const = 0;

    virtual bool isCompileTimeExpr() const = 0;
    virtual boost::optional<int> getCompileTimeValue() const = 0;
};

using ASTExprNode = std::unique_ptr<ExprNode>;

#endif
