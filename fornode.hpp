#ifndef _FORNODE_HPP_
#define _FORNODE_HPP_

#include <vector>

#include "exprnode.hpp"
#include "codegen.hpp"
#include "localscope.hpp"

using std::vector;

class ForNode : public AST
{
public:

	ForNode(AST *init, ExprNode *cond, AST *step, AST *stats);

	virtual	~ForNode();

	virtual void build_scope();
	virtual void define(const TemplateInfo& template_info);
	virtual void check(const TemplateInfo& template_info);
	virtual void gen(const TemplateInfo& template_info);

	virtual AST* copyTree() const;
	virtual vector<AST*> getChildren() const;

private:
	
	static string getNewLabel();

	AST *init;
	ExprNode *cond;
	AST *step;
    AST	*stats;

	LocalScope *for_scope;
};

#endif
