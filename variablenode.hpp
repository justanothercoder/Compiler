#ifndef _VARIABLENODE_HPP_
#define _VARIABLENODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctiontypeinfo.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"
#include "structsymbol.hpp"

class VariableNode : public ExprNode
{
public:

    VariableNode(string name);

    virtual Type *getType() const;
    virtual bool isLeftValue() const;    

    virtual void build_scope();
    virtual void check();
    virtual void gen();

private:

    string name;
    
    VariableSymbol *variable;
};

#endif
