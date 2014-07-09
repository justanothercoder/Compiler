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
	~BinaryOperatorNode() override;

    void check() override;
    CodeObject& gen() override;

    string getOperatorName();
    string getCodeOperatorName();

	vector<AST*> getChildren() const override;
    AST* copyTree() const override;

    VariableType getType() const override;
	bool isLeftValue() const override;

	int neededSpaceForTemporaries() override;

	void freeTempSpace() override;

protected:

    ExprNode *lhs, *rhs;
    BinaryOp op_type;
    CallInfo call_info;
	CodeObject code_obj;
};

#endif
