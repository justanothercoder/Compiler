#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "codegen.hpp"

class ReturnNode : public AST
{
public:

    ReturnNode(ExprNode *expr);
	
	virtual ~ReturnNode();

    virtual AST* copyTree() const;

    virtual void build_scope();

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

	virtual vector<AST*> getChildren() const;

private:

    ExprNode *expr;
    
};

#endif
