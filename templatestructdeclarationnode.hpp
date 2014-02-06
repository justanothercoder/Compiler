#ifndef _TEMPLATESTRUCTDECLARATIONNODE_HPP_
#define _TEMPLATESTRUCTDECLARATIONNODE_HPP_

#include "structdeclarationnode.hpp"
#include "typeinfo.hpp"
#include "templatestructsymbol.hpp"
#include "templatedeclholder.hpp"

class TemplateStructDeclarationNode : public StructDeclarationNode, public TemplateDeclHolder
{
public:

    TemplateStructDeclarationNode(string name, const vector<DeclarationNode*>& inner, const vector< pair<string, TypeInfo> >& template_params);

    virtual void build_scope();
    virtual void define();
    virtual void check();
    
    virtual void template_check(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr);

    virtual void visitChildren();
    
private:

    vector< pair<string, TypeInfo> > template_params;
};

#endif
