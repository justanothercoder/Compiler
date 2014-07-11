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

	~ForNode() override;

	void build_scope() override;

	void define() override;
	void check() override;

	CodeObject& gen() override;

	AST* copyTree() const override;

	vector<AST*> getChildren() const override;

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
