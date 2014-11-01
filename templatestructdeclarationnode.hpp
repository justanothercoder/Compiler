#ifndef _TEMPLATESTRUCTDECLARATIONNODE_HPP_
#define _TEMPLATESTRUCTDECLARATIONNODE_HPP_

#include "structdeclarationnode.hpp"
#include "typeinfo.hpp"

class TemplateStructDeclarationNode : public StructDeclarationNode
{
public:

    TemplateStructDeclarationNode(std::string name, std::vector<AST*> inner, std::vector< std::pair<std::string, TypeInfo> > template_params);

    void build_scope() override;

private:

    std::vector< std::pair<std::string, TypeInfo> > template_params;
};

#endif
