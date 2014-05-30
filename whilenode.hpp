#ifndef _WHILENODE_HPP_
#define _WHILENODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codeobject.hpp"

class WhileNode : public AST
{
public:

    WhileNode(ExprNode *cond, AST *stats);

	virtual ~WhileNode();

    virtual AST* copyTree() const;

    virtual void build_scope();

    virtual void define();
    virtual void check();
    virtual CodeObject& gen();
   
   	virtual vector<AST*> getChildren() const;
    
private:

    ExprNode *cond;
    AST *stats;

    LocalScope *while_scope;

	CodeObject code_obj;

    static string getNewLabel();
};

#endif
