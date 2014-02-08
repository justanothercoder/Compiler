#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codegen.hpp"

class IfNode : public AST
{
public:

    IfNode(ExprNode *cond, AST *stats_true, AST *stats_false);

    virtual AST* copyTree() const;

    virtual void build_scope();
    virtual void define();
    virtual void check();
    virtual void gen();

    virtual void template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual void template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
	
private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    LocalScope *if_scope, *else_scope;
    
    static int label_num;
    static string getNewLabel();
};

#endif
