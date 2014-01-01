#ifndef _ASSIGNMENTNODE_HPP_
#define _ASSIGNMENTNODE_HPP_

#include "exprnode.hpp"

class AssignmentNode : public AST
{
public:

    AssignmentNode(ExprNode *lhs, ExprNode *rhs);

    virtual void build_scope();
    virtual void define();
    virtual void check();
    virtual void gen();
    
private:

    ExprNode *lhs, *rhs;
    
};

#endif
