#ifndef _EXPRNODE_HPP_
#define _EXPRNODE_HPP_

#include "ast.hpp"
#include "type.hpp"
#include "codegen.hpp"

class ExprNode : public AST
{
public:

    virtual void build_scope();
    virtual void define();
    
    virtual Type* getType() = 0;
    virtual bool isLeftValue() = 0;
};

#endif
