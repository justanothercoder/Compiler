#ifndef _WHILENODE_HPP_
#define _WHILENODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codegen.hpp"

class WhileNode : public AST
{
public:

    WhileNode(ExprNode *cond, AST *stats);

	virtual ~WhileNode();

    virtual AST* copyTree() const;

    virtual void build_scope();

    virtual void define(const TemplateInfo& template_info);
    virtual void check(const TemplateInfo& template_info);
    virtual void gen(const TemplateInfo& template_info);
   
   	virtual vector<AST*> getChildren() const;
    
private:

    ExprNode *cond;
    AST *stats;

    LocalScope *while_scope;

    static string getNewLabel();
};

#endif
