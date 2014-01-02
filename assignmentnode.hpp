#ifndef _ASSIGNMENTNODE_HPP_
#define _ASSIGNMENTNODE_HPP_

#include "exprnode.hpp"
#include "overloadedfunctiontype.hpp"
#include "typedsymbol.hpp"
#include "functionhelper.hpp"
#include "functionsymbol.hpp"

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

    FunctionSymbol *resolved_function_symbol;
    
};

#endif
