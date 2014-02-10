#ifndef _BINARYOPERATORNODE_HPP_
#define _BINARYOPERATORNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "referencetype.hpp"
#include "functionhelper.hpp"
#include "codegen.hpp"

enum class BinaryOp { ASSIGN, PLUS, MINUS, MUL };

class BinaryOperatorNode : public ExprNode
{
public:

    BinaryOperatorNode(ExprNode *lhs, ExprNode *rhs, BinaryOp op_type);

    virtual AST* copyTree() const;

    virtual void build_scope();
    
    virtual bool isLeftValue() const;
    virtual Type *getType() const;

    string getOperatorName();
    string getCodeOperatorName();

    void special_check();

    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

protected:

    ExprNode *lhs, *rhs;
    FunctionSymbol *resolved_operator_symbol;
    BinaryOp op_type;
};

#endif
