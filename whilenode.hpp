#ifndef _WHILENODE_HPP_
#define _WHILENODE_HPP_

#include "exprnode.hpp"
#include "localscope.hpp"
#include "codegen.hpp"

class WhileNode : public AST
{
public:

    WhileNode(ExprNode *cond, AST *stats);

    virtual void build_scope();
    virtual void define();
    virtual void check();
    virtual void gen();    

    virtual void template_check(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);
    virtual bool isTemplated() const;
    
private:

    ExprNode *cond;
    AST *stats;

    LocalScope *while_scope;

    static int label_num;
    static string getNewLabel();
};

#endif
