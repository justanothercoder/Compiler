#ifndef _IFNODE_HPP_
#define _IFNODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codeobject.hpp"

class IfNode : public AST
{
public:

    IfNode(ExprNode *cond, AST *stats_true, AST *stats_false);

	virtual ~IfNode();

    virtual AST* copyTree() const;

    virtual void build_scope();

    virtual void define();
    virtual void check();
    virtual CodeObject& gen();
	
	virtual vector<AST*> getChildren() const;
		
private:

    ExprNode *cond;
    AST *stats_true, *stats_false;

    LocalScope *if_scope, *else_scope;

	CodeObject code_obj;

    static string getNewLabel();
};

#endif
