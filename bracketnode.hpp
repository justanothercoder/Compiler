#ifndef _BRACKETNODE_HPP_
#define _BRACKETNODE_HPP_

#include "exprnode.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "functionhelper.hpp"
#include "codegen.hpp"

class BracketNode : public ExprNode
{
public:
	
	BracketNode(ExprNode *base, ExprNode *expr);

	virtual ~BracketNode();

	virtual void build_scope();
	
	virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
	virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);	

	virtual Type* getType() const;
	virtual bool isLeftValue() const;

	virtual AST* copyTree() const;

private:

	ExprNode *base, *expr;

	FunctionSymbol *resolved_operator;
};

#endif
