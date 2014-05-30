#ifndef _FORNODE_HPP_
#define _FORNODE_HPP_

#include <vector>

#include "exprnode.hpp"
#include "codeobject.hpp"
#include "localscope.hpp"

using std::vector;

class ForNode : public AST
{
public:

	ForNode(AST *init, ExprNode *cond, AST *step, AST *stats);

	virtual	~ForNode();

	virtual void build_scope();
	virtual void define();
	virtual void check();
	virtual CodeObject& gen();

	virtual AST* copyTree() const;
	virtual vector<AST*> getChildren() const;

private:
	
	static string getNewLabel();

	AST *init;
	ExprNode *cond;
	AST *step;
    AST	*stats;

	LocalScope *for_scope;
	CodeObject code_obj;
};

#endif
