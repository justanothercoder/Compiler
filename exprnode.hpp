#ifndef _EXPRNODE_HPP_
#define _EXPRNODE_HPP_

#include "ast.hpp"
#include "variabletype.hpp"

class ExprNode : public AST
{
public:

    virtual void define(const TemplateStructSymbol *sym, std::vector<ExprNode*> expr);
    
    virtual VariableType getType() const = 0;
    
	bool isLeftValue() const;
};

#endif
