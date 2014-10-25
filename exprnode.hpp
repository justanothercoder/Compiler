#ifndef _EXPRNODE_HPP_
#define _EXPRNODE_HPP_

#include <boost/optional.hpp>

#include "ast.hpp"
#include "logger.hpp"

class Type;

class ExprNode : public AST
{
public:

	ExprNode();

    virtual const Type* getType() const = 0;
	virtual bool isLeftValue() const = 0;

	virtual void freeTempSpace() = 0;

	virtual bool isCompileTimeExpr() const = 0;
	virtual boost::optional<int> getCompileTimeValue() const = 0;

	const Type *type_hint;
};

#endif
