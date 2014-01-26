#ifndef _EXPRNODE_HPP_
#define _EXPRNODE_HPP_

#include "ast.hpp"
#include "type.hpp"

class ExprNode : public AST
{
public:

    virtual void define();
    
    virtual Type* getType() const = 0;
    virtual bool isLeftValue() const = 0;
};

#endif
