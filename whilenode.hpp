#ifndef _WHILENODE_HPP_
#define _WHILENODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codeobject.hpp"

class WhileNode : public AST
{
public:

    WhileNode(ExprNode *cond, AST *stats);

	~WhileNode() override;

    AST* copyTree() const override;

    void build_scope() override;

    void define() override;
    void check() override;

    CodeObject& gen() override;
   
   	vector<AST*> getChildren() const override;

	int neededSpaceForTemporaries() override;
    
private:

    ExprNode *cond;
    AST *stats;

    LocalScope *while_scope;

	CodeObject code_obj;

    static string getNewLabel();
};

#endif
