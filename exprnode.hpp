#ifndef _EXPRNODE_HPP_
#define _EXPRNODE_HPP_

#include "ast.hpp"
#include "variabletype.hpp"

#include "logger.hpp"

class ExprNode : public AST
{
public:

	ExprNode();

    virtual void define();
    
    virtual VariableType getType() const = 0;
	virtual bool isLeftValue() const = 0;

	Type *type_hint;
};

#endif
