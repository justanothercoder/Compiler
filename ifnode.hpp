#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codeobject.hpp"

class IfNode : public AST
{
public:

    IfNode(ExprNode *cond, AST *stats_true, AST *stats_false);

	~IfNode() override;

    AST* copyTree() const override;

    void build_scope() override;

    void define() override;
    void check() override;

    CodeObject& gen() override;
	
	vector<AST*> getChildren() const override;

private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    LocalScope *if_scope, *else_scope;

	CodeObject code_obj;

    static string getNewLabel();
};

#endif
