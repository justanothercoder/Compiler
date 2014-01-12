#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "referencetype.hpp"
#include "functionhelper.hpp"

enum class BinaryOp { ASSIGN, PLUS };

class BinaryOperatorNode : public ExprNode
{
public:

    BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type);

    virtual void build_scope();
    virtual void check();
    virtual void gen();
    
    virtual bool isLeftValue();
    virtual Type *getType();

    string getOperatorName();
    string getCodeOperatorName();

    void special_check();
    
protected:

    ExprNode *lhs, *rhs;
    FunctionSymbol *resolved_operator_symbol;
    BinaryOp op_type;
};

#endif
