#ifndef _TEMPLATESTRUCTDECLARATIONNODE_HPP_
#define _TEMPLATESTRUCTDECLARATIONNODE_HPP_

#include "structdeclarationnode.hpp"
#include "typeinfo.hpp"
#include "templatestructsymbol.hpp"

class TemplateStructDeclarationNode : public StructDeclarationNode
{
public:

    TemplateStructDeclarationNode(string name, const vector<DeclarationNode*>& inner, const vector< pair<string, TypeInfo> >& template_params);

    virtual void build_scope();
	    
    virtual void define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);
    virtual void gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr);

    virtual Scope* getDeclScope() const;

	virtual vector<AST*> getChildren() const;
    
private:

    vector< pair<string, TypeInfo> > template_params;
};

#endif
