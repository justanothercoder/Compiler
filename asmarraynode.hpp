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

	virtual void define(const TemplateInfo& template_info);
	virtual void check(const TemplateInfo& template_info);
	virtual void gen(const TemplateInfo& template_info);
	
	virtual AST* copyTree() const;
	virtual std::vector<AST*> getChildren() const;

private:
	
	int size_of_type;
	int array_size;
};

#endif
