#ifndef _NUMBERNODE_HPP_
#define _NUMBERNODE_HPP_

#include "exprnode.hpp"

class NumberNode : public ExprNode
{
public:

    NumberNode(string num);

    virtual void build_scope();
    virtual void gen();
    
    virtual void check();
    virtual Type* getType();
    virtual bool isLeftValue();
    
private:

    static Type* expr_type;
    
    string num;
    
};
    
#endif
