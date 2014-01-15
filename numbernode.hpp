#ifndef _NUMBERNODE_HPP_
#define _NUMBERNODE_HPP_

#include "exprnode.hpp"
#include "globalconfig.hpp"

class NumberNode : public ExprNode
{
public:

    NumberNode(string num);

    virtual void build_scope();
    virtual void check();    
    virtual void gen();
    
    virtual Type* getType() const;
    virtual bool isLeftValue() const;
    
private:

    string num;
    
};
    
#endif
