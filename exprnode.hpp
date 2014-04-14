#ifndef _EXPRNODE_HPP_
#define _EXPRNODE_HPP_

#include "ast.hpp"
#include "variabletype.hpp"

class ExprNode : public AST
{
public:

    virtual void define(const TemplateInfo& template_info);
    
    virtual VariableType getType() const = 0;
	virtual bool isLeftValue() const = 0;
};

#endif
