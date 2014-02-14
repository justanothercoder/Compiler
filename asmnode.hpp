#ifndef _ASMNODE_HPP_
#define _ASMNODE_HPP_

#include <string>

#include "ast.hpp"
#include "codegen.hpp"
#include "declarationnode.hpp"

using std::string;

class AsmNode : public DeclarationNode
{
public:
	
	AsmNode(string code);
	
	virtual void build_scope();

    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) = 0;
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) = 0;
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) = 0;

private:

	string code;
};

#endif
