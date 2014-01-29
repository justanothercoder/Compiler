#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "codegen.hpp"

class ReturnNode : public AST
{
public:

    ReturnNode(ExprNode *expr);

    virtual void build_scope();
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual void template_check();    

private:

    ExprNode *expr;
    FunctionSymbol *func;
    
};

#endif
