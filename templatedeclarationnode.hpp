#ifndef _TEMPLATEDECLARATIONNODE_HPP_
#define _TEMPLATEDECLARATIONNODE_HPP_

#include "declarationnode.hpp"
#include "templateinfo.hpp"

class TemplateDeclarationNode : public DeclarationNode
{
public:
    virtual void addInstance(std::vector<TemplateParam> template_params, DeclarationNode* decl) = 0;
    virtual DeclarationNode* getInstance(std::vector<TemplateParam> template_params) const = 0;
    virtual DeclarationNode* instantiateWithTemplateInfo(TemplateInfo info) = 0;
    virtual std::vector<DeclarationNode*> allInstances() const = 0;
};

#endif
