#ifndef _TEMPLATEDECLARATIONNODE_HPP_
#define _TEMPLATEDECLARATIONNODE_HPP_

#include <memory>
#include "declarationnode.hpp"
#include "templateinfo.hpp"

class TemplateDeclarationNode : public DeclarationNode
{
public:
    virtual void addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> decl) = 0;
    virtual std::shared_ptr<DeclarationNode> getInstance(std::vector<TemplateParam> template_params) const = 0;    
    virtual std::shared_ptr<DeclarationNode> instantiateWithParams(std::vector<TemplateParam> params) = 0;
    virtual std::vector<DeclarationNode*> allInstances() const = 0;
};

#endif
