#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"
#include "typedsymbol.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"

class VariableNode : public ExprNode
{
public:

    VariableNode(string name);

    virtual Type *getType();

    virtual void build_scope();
    virtual void check();
    virtual void gen();

    virtual bool isLeftValue();       
    
private:

    string name;
    
    TypedSymbol *variable;
};

#endif
