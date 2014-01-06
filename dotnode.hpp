#ifndef _DOTNODE_HPP_
#define _DOTNODE_HPP_

#include "exprnode.hpp"
#include "structsymbol.hpp"

class DotNode : public ExprNode
{
public:

    DotNode(ExprNode *base, string member_name);   

    virtual void build_scope();
    virtual void check();
    virtual void gen();

    virtual Type* getType();
    virtual bool isLeftValue();
    
private:

    ExprNode *base;
    string member_name;

    Symbol *member;
};

#endif
