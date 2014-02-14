#ifndef _ASMARRAYNODE_HPP_
#define _ASMARRAYNODE_HPP_

#include "asmnode.hpp"

class AsmArrayNode : public AsmNode
{
public:

	AsmArrayNode(int size);

	virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

private:
	int size;
};

#endif
