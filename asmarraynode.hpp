#ifndef _ASMARRAYNODE_HPP_
#define _ASMARRAYNODE_HPP_

#include "codegen.hpp"
#include "classvariablesymbol.hpp"
#include "templatesymbol.hpp"
#include "numbernode.hpp"
#include "functionsymboldefine.hpp"
#include "variablesymboldefine.hpp"

class AsmArrayNode : public AST
{
public:
	
	AsmArrayNode();

	virtual void build_scope();

	virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
	virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
	virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
	
	virtual AST* copyTree() const;
	virtual std::vector<AST*> getChildren() const;

private:
	
	int size_of_type;
	int array_size;
};

#endif
