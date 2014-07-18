#ifndef _TEMPLATESTRUCTDECLARATIONNODE_HPP_
#define _TEMPLATESTRUCTDECLARATIONNODE_HPP_

#include "structdeclarationnode.hpp"
#include "typeinfo.hpp"
#include "templatestructsymbol.hpp"

class TemplateStructDeclarationNode : public StructDeclarationNode
{
public:

    TemplateStructDeclarationNode(string name, const std::vector<AST*>& inner, const std::vector< std::pair<string, TypeInfo> >& template_params);

    virtual void build_scope();
	    
    virtual void define();
    virtual void check();
    virtual CodeObject& gen();

private:

	std::vector< std::pair<string, TypeInfo> > template_params;
};

#endif
