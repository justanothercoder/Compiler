#ifndef _RETURNNODE_HPP_
#define _RETURNNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "globalconfig.hpp"
#include "codegen.hpp"
#include "callinfo.hpp"

class ReturnNode : public AST
{
public:

    ReturnNode(ExprNode *expr);
	
	virtual ~ReturnNode();

    virtual AST* copyTree() const;

	virtual void define(const TemplateInfo& template_info);
	virtual void check(const TemplateInfo& template_info);
    virtual void gen(const TemplateInfo& template_info);

	virtual vector<AST*> getChildren() const;

private:

    ExprNode *expr;

	CallInfo copy_call_info;
};

#endif
