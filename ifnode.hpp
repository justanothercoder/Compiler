#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include "exprnode.hpp"

class IfNode : public AST
{
public:

    IfNode(ExprNode *cond, AST *stats_true, AST *stats_false);

    virtual void build_scope();
    virtual void define();
    virtual void check();
    virtual void gen();
    
private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    static int label_num;
    static string getNewLabel();
};

#endif
