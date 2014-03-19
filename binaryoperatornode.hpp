#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "functionhelper.hpp"
#include "codegen.hpp"
#include "callhelper.hpp"
#include "builtins.hpp"

enum class BinaryOp { ASSIGN, PLUS, MINUS, MUL };

class BinaryOperatorNode : public ExprNode
{
public:

    BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type);

	virtual ~BinaryOperatorNode();

    virtual AST* copyTree() const;

    virtual VariableType getType() const;

    string getOperatorName();
    string getCodeOperatorName();

    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;

protected:

    ExprNode *lhs, *rhs;
    BinaryOp op_type;
    CallInfo call_info;
};

#endif
