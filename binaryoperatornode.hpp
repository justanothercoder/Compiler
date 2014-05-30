#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "functionhelper.hpp"
#include "codeobject.hpp"
#include "callhelper.hpp"
#include "builtins.hpp"

enum class BinaryOp { ASSIGN, PLUS, MINUS, MUL, EQUALS, NEQUALS, AND, OR, DIV, MOD };

class BinaryOperatorNode : public ExprNode
{
public:

    BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type);
	virtual ~BinaryOperatorNode();

    virtual void check();
    virtual CodeObject& gen();

    string getOperatorName();
    string getCodeOperatorName();

	virtual vector<AST*> getChildren() const;
    virtual AST* copyTree() const;

    virtual VariableType getType() const;
	virtual bool isLeftValue() const;

protected:

    ExprNode *lhs, *rhs;
    BinaryOp op_type;
    CallInfo call_info;
	CodeObject code_obj;
};

#endif
