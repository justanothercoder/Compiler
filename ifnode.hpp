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

    virtual void define(const TemplateInfo& template_info);
    virtual void check(const TemplateInfo& template_info);
    virtual void gen(const TemplateInfo& template_info);
	
	virtual vector<AST*> getChildren() const;
		
private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    LocalScope *if_scope, *else_scope;
    
    static string getNewLabel();
};

#endif
