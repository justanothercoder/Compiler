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

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    
private:

    ExprNode *cond;
    AST *stats;

    LocalScope *while_scope;

    static int label_num;
    static string getNewLabel();
};

#endif
