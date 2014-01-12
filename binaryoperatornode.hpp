#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "referencetype.hpp"
#include "functionhelper.hpp"

class BinaryOperatorNode : public ExprNode
{
public:

    BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs);

    virtual void build_scope();
    virtual void check();
    
    virtual bool isLeftValue();
    virtual Type *getType();

    virtual string getOperatorName() = 0;

protected:

    ExprNode *lhs, *rhs;
    FunctionSymbol *resolved_operator_symbol;
};

#endif
