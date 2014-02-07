#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "codegen.hpp"

class ReturnNode : public AST
{
public:

    ReturnNode(ExprNode *expr);

    virtual AST* copyTree() const;

    virtual void build_scope();
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual void template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual bool isTemplated() const;

private:

    ExprNode *expr;
    FunctionSymbol *func;
    
};

#endif
