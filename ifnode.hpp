#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codegen.hpp"

class IfNode : public AST
{
public:

    IfNode(ExprNode *cond, AST *stats_true, AST *stats_false);

	virtual ~IfNode();

    virtual AST* copyTree() const;

    virtual void build_scope();

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
	
private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    LocalScope *if_scope, *else_scope;
    
    static int label_num;
    static string getNewLabel();
};

#endif
