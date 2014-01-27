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
    virtual void define();
    
private:

    vector< pair<string, TypeInfo> > template_params;
};

#endif
