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

	~ForNode();

	virtual void build_scope();
	virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
	virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
	virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

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
