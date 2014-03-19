#ifndef _DECLARATIONNODE_HPP_
#define _DECLARATIONNODE_HPP_

#include "ast.hpp"

class Symbol;

class DeclarationNode : public AST
{
public:

    virtual ~DeclarationNode();    

	virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> template_expr);

    virtual Symbol* getDefinedSymbol() const = 0;
};

#endif
