#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"
#include "typedsymbol.hpp"

class VariableNode : public ExprNode
{
public:

    VariableNode(string name);

    virtual Type *getType();
    
    virtual void check();
    virtual void gen();

    virtual bool isLeftValue();       
    
private:

    string name;
    
    TypedSymbol *variable;
};

#endif
